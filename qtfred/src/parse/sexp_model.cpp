#include "sexpModel.h"
#include "mission/util.h"
#include "mission/Editor.h"
#include "mission/object.h"

#include "parse/sexp.h"
#include "globalincs/linklist.h"
#include "ai/aigoals.h"
#include "ai/ailua.h"
#include "asteroid/asteroid.h"
#include "mission/missionmessage.h"
#include "mission/missioncampaign.h"
#include "mission/missionparse.h"
#include "hud/hudsquadmsg.h"
#include "stats/medals.h"
#include "controlconfig/controlsconfig.h"
#include "hud/hudgauges.h"
#include "starfield/starfield.h"
#include "nebula/neb.h"
#include "nebula/neblightning.h"
#include "jumpnode/jumpnode.h"
#include "gamesnd/eventmusic.h"    // for change-soundtrack
#include "menuui/techmenu.h"    // for intel stuff
#include "weapon/emp.h"
#include "gamesnd/gamesnd.h"
#include "weapon/weapon.h"
#include "hud/hudartillery.h"
#include "iff_defs/iff_defs.h"
#include "mission/missionmessage.h"
#include "sound/ds.h"
#include "globalincs/alphacolors.h"
#include "localization/localize.h"
#include "mission/missiongoals.h"
#include "ship/ship.h"


// FINISH ME!
void sexp_tree::get_node_from_id{
	return 0;
}

// clears out the tree, so all the nodes are unused.
// total_nodes removed
void sexp_tree::clear_tree(const char* op) {
	mprintf(("Resetting dynamic tree node limit from "
				SIZE_T_ARG
				" to %d...\n", tree_nodes.size(), 0));

	flag = 0;
	tree_nodes.clear();

	if (op) {
		clear();
		if (strlen(op)) {
			set_node(allocate_node(-1), (SEXPT_OPERATOR | SEXPT_VALID), op);
			build_tree();
		}
	}
}

// initializes and creates a tree from a given sexp startpoint.
// No original code during qtFred UI separation
void sexp_tree::load_tree(int index, const char* default_text) {
	int cur;

	clear_tree();
	root_item = 0;
	if (index < 0) {
		cur = allocate_node(-1);
		set_node(cur, (SEXPT_OPERATOR | SEXPT_VALID), default_text);  // setup a default tree if none
		build_tree();
		return;
	}

	if (Sexp_nodes[index].subtype == SEXP_ATOM_NUMBER) {  // handle numbers allender likes to use so much..
		cur = allocate_node(-1);
		if (atoi(Sexp_nodes[index].text)) {
			set_node(cur, (SEXPT_OPERATOR | SEXPT_VALID), "true");
		} else {
			set_node(cur, (SEXPT_OPERATOR | SEXPT_VALID), "false");
		}

		build_tree();
		return;
	}

	// assumption: first token is an operator.  I require this because it would cause problems
	// with child/parent relations otherwise, and it should be this way anyway, since the
	// return type of the whole sexp is boolean, and only operators can satisfy this.
	Assert(Sexp_nodes[index].subtype == SEXP_ATOM_OPERATOR);
	load_branch(index, -1);
	build_tree();
}

void get_combined_variable_name(char* combined_name, const char* sexp_var_name) {
	int sexp_var_index = get_index_sexp_variable_name(sexp_var_name);

	if (sexp_var_index >= 0)
		sprintf(combined_name, "%s(%s)", Sexp_variables[sexp_var_index].variable_name, Sexp_variables[sexp_var_index].text);
	else
		sprintf(combined_name, "%s(undefined)", sexp_var_name);
}

int sexp_tree::load_branch(int index, int parent) {
	int cur = -1;
	char combined_var_name[2 * TOKEN_LENGTH + 2];

	while (index != -1) {
		int additional_flags = SEXPT_VALID;

		// special check for container modifiers
		if ((parent != -1) && (tree_nodes[parent].type & SEXPT_CONTAINER_DATA)) {
			additional_flags |= SEXPT_MODIFIER;
		}

		Assert(Sexp_nodes[index].type != SEXP_NOT_USED);
		if (Sexp_nodes[index].subtype == SEXP_ATOM_LIST) {
			load_branch(Sexp_nodes[index].first, parent);  // do the sublist and continue

		} else if (Sexp_nodes[index].subtype == SEXP_ATOM_OPERATOR) {
			cur = allocate_node(parent);
			if ((index == select_sexp_node) && !flag) {  // translate sexp node to our node
				select_sexp_node = cur;
				flag = 1;
			}

			set_node(cur, (SEXPT_OPERATOR | additional_flags), Sexp_nodes[index].text);
			load_branch(Sexp_nodes[index].rest, cur);  // operator is new parent now
			return cur;  // 'rest' was just used, so nothing left to use.

		} else if (Sexp_nodes[index].subtype == SEXP_ATOM_NUMBER) {
			cur = allocate_node(parent);
			if (Sexp_nodes[index].type & SEXP_FLAG_VARIABLE) {
				get_combined_variable_name(combined_var_name, Sexp_nodes[index].text);
				set_node(cur, (SEXPT_VARIABLE | SEXPT_NUMBER | additional_flags), combined_var_name);
			} else {
				set_node(cur, (SEXPT_NUMBER | additional_flags), Sexp_nodes[index].text);
			}

		} else if (Sexp_nodes[index].subtype == SEXP_ATOM_STRING) {
			cur = allocate_node(parent);
			if (Sexp_nodes[index].type & SEXP_FLAG_VARIABLE) {
				get_combined_variable_name(combined_var_name, Sexp_nodes[index].text);
				set_node(cur, (SEXPT_VARIABLE | SEXPT_STRING | additional_flags), combined_var_name);
			}  else {
				set_node(cur, (SEXPT_STRING | additional_flags), Sexp_nodes[index].text);
			}

		} else if (Sexp_nodes[index].subtype == SEXP_ATOM_CONTAINER_NAME) {
			Assertion(!(additional_flags & SEXPT_MODIFIER),
				"Found a container name node %s that is also a container modifier. Please report!",
				Sexp_nodes[index].text);
			Assertion(get_sexp_container(Sexp_nodes[index].text) != nullptr,
				"Attempt to load unknown container data %s into SEXP tree. Please report!",
				Sexp_nodes[index].text);
			cur = allocate_node(parent);
			set_node(cur, (SEXPT_CONTAINER_NAME | SEXPT_STRING | additional_flags), Sexp_nodes[index].text);

		} else if (Sexp_nodes[index].subtype == SEXP_ATOM_CONTAINER_DATA) {
			cur = allocate_node(parent);
			Assertion(get_sexp_container(Sexp_nodes[index].text) != nullptr,
				"Attempt to load unknown container %s into SEXP tree. Please report!",
				Sexp_nodes[index].text);
			set_node(cur, (SEXPT_CONTAINER_DATA | SEXPT_STRING | additional_flags), Sexp_nodes[index].text);
			load_branch(Sexp_nodes[index].first, cur);  // container is new parent now

		} else
			Assert(0);  // unknown and/or invalid sexp type

		if ((index == select_sexp_node) && !flag) {  // translate sexp node to our node
			select_sexp_node = cur;
			flag = 1;
		}

		index = Sexp_nodes[index].rest;
		if (index == -1) {
			return cur;
		}
	}

	return cur;
}

int sexp_tree::query_false(int node) {
	if (node < 0) {
		node = root_item;
	}

	Assert(node >= 0);
	Assert(tree_nodes[node].type == (SEXPT_OPERATOR | SEXPT_VALID));
	Assert(tree_nodes[node].next == -1);  // must make this assumption or else it will confuse code!
	if (get_operator_const(tree_nodes[node].text) == OP_FALSE) {
		return TRUE;
	}

	return FALSE;
}

// builds an sexp of the tree and returns the index of it.  This allocates sexp nodes.
int sexp_tree::save_tree(int node) {
	if (node < 0) {
		node = root_item;
	}

	Assert(node >= 0);
	Assert(tree_nodes[node].type == (SEXPT_OPERATOR | SEXPT_VALID));
	Assert(tree_nodes[node].next == -1);  // must make this assumption or else it will confuse code!
	return save_branch(node);
}

// get variable name from sexp_tree node .text
void var_name_from_sexp_tree_text(char* var_name, const char* text) {
	auto var_name_length = strcspn(text, "(");
	Assert(var_name_length < TOKEN_LENGTH - 1);

	strncpy(var_name, text, var_name_length);
	var_name[var_name_length] = '\0';
}


#define NO_PREVIOUS_NODE -9
// called recursively to save a tree branch and everything under it
// SEXPT_CONTAINER_NAME and SEXPT_MODIFIER require no special handling here
int sexp_tree::save_branch(int cur, int at_root) {
	int start, node = -1, last = NO_PREVIOUS_NODE;
	char var_name_text[TOKEN_LENGTH];

	start = -1;
	while (cur != -1) {
		if (tree_nodes[cur].type & SEXPT_OPERATOR) {
			node =
				alloc_sexp(tree_nodes[cur].text, SEXP_ATOM, SEXP_ATOM_OPERATOR, -1, save_branch(tree_nodes[cur].child));

			if ((tree_nodes[cur].parent >= 0) && !at_root) {
				node = alloc_sexp("", SEXP_LIST, SEXP_ATOM_LIST, node, -1);
			}
		} else if (tree_nodes[cur].type & SEXPT_CONTAINER_NAME) {
			Assertion(get_sexp_container(tree_nodes[cur].text) != nullptr,
				"Attempt to save unknown container %s from SEXP tree. Please report!",
				tree_nodes[cur].text);
			node = alloc_sexp(tree_nodes[cur].text, SEXP_ATOM, SEXP_ATOM_CONTAINER_NAME, -1, -1);
		} else if (tree_nodes[cur].type & SEXPT_CONTAINER_DATA) {
			Assertion(get_sexp_container(tree_nodes[cur].text) != nullptr,
				"Attempt to save unknown container %s from SEXP tree. Please report!",
				tree_nodes[cur].text);
			node = alloc_sexp(tree_nodes[cur].text, SEXP_ATOM, SEXP_ATOM_CONTAINER_DATA, save_branch(tree_nodes[cur].child), -1);
		} else if (tree_nodes[cur].type & SEXPT_NUMBER) {
			// allocate number, maybe variable
			if (tree_nodes[cur].type & SEXPT_VARIABLE) {
				var_name_from_sexp_tree_text(var_name_text, tree_nodes[cur].text);
				node = alloc_sexp(var_name_text, (SEXP_ATOM | SEXP_FLAG_VARIABLE), SEXP_ATOM_NUMBER, -1, -1);
			} else {
				node = alloc_sexp(tree_nodes[cur].text, SEXP_ATOM, SEXP_ATOM_NUMBER, -1, -1);
			}
		} else if (tree_nodes[cur].type & SEXPT_STRING) {
			// allocate string, maybe variable
			if (tree_nodes[cur].type & SEXPT_VARIABLE) {
				var_name_from_sexp_tree_text(var_name_text, tree_nodes[cur].text);
				node = alloc_sexp(var_name_text, (SEXP_ATOM | SEXP_FLAG_VARIABLE), SEXP_ATOM_STRING, -1, -1);
			} else {
				node = alloc_sexp(tree_nodes[cur].text, SEXP_ATOM, SEXP_ATOM_STRING, -1, -1);
			}
		} else {
			Assert(0); // unknown and/or invalid type
		}

		if (last == NO_PREVIOUS_NODE) {
			start = node;
		} else if (last >= 0) {
			Sexp_nodes[last].rest = node;
		}

		last = node;
		Assert(last != NO_PREVIOUS_NODE);  // should be impossible
		cur = tree_nodes[cur].next;
		if (at_root) {
			return start;
		}
	}

	return start;
}

// find the next free tree node and return its index.
// minor cleanup only
int sexp_tree::find_free_node() {

	for (int i = 0; i < static_cast<int>(tree_nodes.size()); i++) {
		if (tree_nodes[i].type == SEXPT_UNUSED) {
			return i;
		}
	}

	return -1;
}

// allocate a node.  Remains used until freed.
// total_nodes removed
int sexp_tree::allocate_node() {
	int node = find_free_node();

	// need more tree nodes?
	if (node < 0) {
		int old_size = (int) tree_nodes.size();

		Assert(TREE_NODE_INCREMENT > 0);

		// allocate in blocks of TREE_NODE_INCREMENT
		tree_nodes.resize(tree_nodes.size() + TREE_NODE_INCREMENT);

		mprintf(("Bumping dynamic tree node limit from %d to "
					SIZE_T_ARG
					"...\n", old_size, tree_nodes.size()));

#ifndef NDEBUG
		for (int i = old_size; i < (int) tree_nodes.size(); i++) {
			sexp_tree_item* item = &tree_nodes[i];
			Assert(item->type == SEXPT_UNUSED);
		}
#endif

		// our new sexp is the first out of the ones we just created
		node = old_size;
	}

	// reset the new node
	tree_nodes[node].type = SEXPT_UNINIT;
	tree_nodes[node].parent = -1;
	tree_nodes[node].child = -1;
	tree_nodes[node].next = -1;
	tree_nodes[node].flags = 0;
	strcpy_s(tree_nodes[node].text, "<uninitialized tree node>");
	tree_nodes[node].handle = NULL;

	return node;
}

// allocate a child node under 'parent'.  Appends to end of list.
int sexp_tree::allocate_node(int parent, int after) {
	int i, index = allocate_node();

	if (parent != -1) {
		i = tree_nodes[parent].child;
		if (i == -1) {
			tree_nodes[parent].child = index;

		} else {
			while ((i != after) && (tree_nodes[i].next != -1)) {
				i = tree_nodes[i].next;
			}

			tree_nodes[index].next = tree_nodes[i].next;
			tree_nodes[i].next = index;
		}
	}

	tree_nodes[index].parent = parent;
	return index;
}

// free a node and all its children.  Also clears pointers to it, if any.
//   node = node chain to free
//   cascade =  false: free just this node and children under it. (default)
//             true: free this node and all siblings after it.
//
// Only changed
void sexp_tree::free_node(int node, bool cascade) {
	int i;

	// clear the pointer to node
	i = tree_nodes[node].parent;
	Assert(i != -1);
	if (tree_nodes[i].child == node) {
		tree_nodes[i].child = tree_nodes[node].next;
	} else {
		i = tree_nodes[i].child;
		while (tree_nodes[i].next != -1) {
			if (tree_nodes[i].next == node) {
				tree_nodes[i].next = tree_nodes[node].next;
				break;
			}

			i = tree_nodes[i].next;
		}
	}

	if (!cascade) {
		tree_nodes[node].next = -1;
	}

	// now free up the node and its children
	free_node2(node);
}

// more simple node freer, which works recursively.  It frees the given node and all siblings
// that come after it, as well as all children of these.  Doesn't clear any links to any of
// these freed nodes, so make sure all links are broken first. (i.e. use free_node() if you can)
// Removed total nodes ... because it's 2025, people, and we have vectors now
void sexp_tree::free_node2(int node) {
	Assert(node != -1);
	Assert(tree_nodes[node].type != SEXPT_UNUSED);
	modified();
	tree_nodes[node].type = SEXPT_UNUSED;
	if (tree_nodes[node].child != -1) {
		free_node2(tree_nodes[node].child);
	}

	if (tree_nodes[node].next != -1) {
		free_node2(tree_nodes[node].next);
	}
}


// initialize the data for a node.  Should be called right after a new node is allocated.
void sexp_tree::set_node(int node, int type, const char* text) {
	Assert(type != SEXPT_UNUSED);
	Assert(tree_nodes[node].type != SEXPT_UNUSED);
	tree_nodes[node].type = type;
	size_t max_length;
	if (type & SEXPT_VARIABLE) {
		max_length = 2 * TOKEN_LENGTH + 2;
	} else if (type & (SEXPT_CONTAINER_NAME | SEXPT_CONTAINER_DATA)) {
		max_length = sexp_container::NAME_MAX_LENGTH + 1;
	} else {
		max_length = TOKEN_LENGTH;
	}
	// TODO, this ASSERT sounds like it should be adjusted to have differing behavior based on input type
	// Shortening the string with a warning might be an option.
	Assert(strlen(text) < max_length);
	strcpy_s(tree_nodes[node].text, text);
}

// TODO, build interface between nodes and IDs
sexp_item_type sexp_tree::get_item_type(int id){
	int node = get_node_from_id(id);

	// TODO, UI does not yet check for this
	if (node < 0)
		return sexp_item_type::ERROR;

	if (tree_nodes[id].type & SEXPT_OPERATOR)
		return sexp_item_type::OPERATOR;
	if (tree_nodes[id].type & SEXPT_VARIABLE) 
		return sexp_item_type::VARIABLE;
	if (tree_nodes[id].type & SEXPT_CONTAINER_NAME)
		return sexp_item_type::CONTAINER_NAME;
	if (tree_nodes[id].type & SEXPT_CONTAINER_DATA) 
		return sexp_item_type::CONTAINER_DATA;
	
	return sexp_item_type::EDITABLE_OTHER;
}

// construct tree nodes for an sexp, adding them to the list and returning first node
int sexp_tree::load_sub_tree(int index, bool valid, const char* text) {
	int cur;

	if (index < 0) {
		cur = allocate_node(-1);
		set_node(cur, (SEXPT_OPERATOR | (valid ? SEXPT_VALID : 0)), text);  // setup a default tree if none
		return cur;
	}

	// assumption: first token is an operator.  I require this because it would cause problems
	// with child/parent relations otherwise, and it should be this way anyway, since the
	// return type of the whole sexp is boolean, and only operators can satisfy this.
	Assert(Sexp_nodes[index].subtype == SEXP_ATOM_OPERATOR);
	cur = load_branch(index, -1);
	return cur;
}


int sexp_tree::count_args(int node) {
	int count = 0;

	while (node != -1) {
		count++;
		node = tree_nodes[node].next;
	}

	return count;
}


// given a tree node, returns the argument type it should be.
// OPF_NULL means no value (or a "void" value) is returned.  OPF_NONE means there shouldn't be any argument at this position at all.
int sexp_tree::query_node_argument_type(int node) const {
	int parent_node = tree_nodes[node].parent;
	if (parent_node < 0) {		// parent nodes are -1 for a top-level operator like 'when'
		return OPF_NULL;
	}

	int argnum = find_argument_number(parent_node, node);
	if (argnum < 0) {
		return OPF_NONE;
	}

	int op_num = get_operator_index(tree_nodes[parent_node].text);
	if (op_num < 0) {
		return OPF_NONE;
	}

	return query_operator_argument_type(op_num, argnum);
}

// Look for the valid operator that is the closest match for 'str' and return the operator
// number of it.  What operators are valid is determined by 'node', and an operator is valid
// if it is allowed to fit at position 'node'
//
const SCP_string &sexp_tree::match_closest_operator(const SCP_string &str, int node) {
	int z, op, arg_num, opf;

	z = tree_nodes[node].parent;
	if (z < 0) {
		return str;
	}

	op = get_operator_index(tree_nodes[z].text);
	if (op < 0) {
		return str;
	}

	// determine which argument we are of the parent
	arg_num = find_argument_number(z, node);
	opf = query_operator_argument_type(op, arg_num);	// check argument type at this position

	// find the best operator
	int best = sexp_match_closest_operator(str, opf);
	if (best < 0)
		return str;
	return Operators[best].text;
}


// adds to or replaces (based on passed in flag) the current operator
void sexp_tree::add_or_replace_operator(int op, int replace_flag) {
	int i, op2;

	if (replace_flag) {
		if (tree_nodes[item_index].flags & OPERAND) {  // are both operators?
			op2 = get_operator_index(tree_nodes[item_index].text);
			Assert(op2 >= 0);
			i = count_args(tree_nodes[item_index].child);
			if ((i >= Operators[op].min) && (i <= Operators[op].max)) {  // are old num args valid?
				while (i--) {
					if (query_operator_argument_type(op2, i)
						!= query_operator_argument_type(op, i)) {  // does each arg match expected type?
						break;
					}
				}

				if (i < 0) {  // everything is ok, so we can keep old arguments with new operator
					set_node(item_index, (SEXPT_OPERATOR | SEXPT_VALID), Operators[op].text.c_str());
					tree_nodes[item_index].handle->setText(0, QString::fromStdString(Operators[op].text));
					tree_nodes[item_index].flags = OPERAND;
					nodeChanged(item_index);
					return;
				}
			}
		}

		replace_operator(Operators[op].text.c_str());

	} else {
		add_operator(Operators[op].text.c_str());
	}

	// fill in all the required (minimum) arguments with default values
	for (i = 0; i < Operators[op].min; i++) {
		add_default_operator(op, i);
	}
}

int sexp_tree::add_default_operator(int op_index, int argnum) {
	char buf[256];
	int index;
	sexp_list_item item;

	index = item_index;
	if (get_default_value(&item, buf, op_index, argnum)) {
		return -1;
	}

	if (item.type & SEXPT_OPERATOR) {
		Assert((item.op >= 0) && (item.op < (int) Operators.size()));
		add_or_replace_operator(item.op);
		item_index = index;
	} else {
		// special case for sexps that take variables
		const int op_type = query_operator_argument_type(op_index, argnum);
		if (op_type == OPF_VARIABLE_NAME) {
			int sexp_var_index = get_index_sexp_variable_name(item.text);
			Assert(sexp_var_index != -1);
			int type = SEXPT_VALID | SEXPT_VARIABLE;
			if (Sexp_variables[sexp_var_index].type & SEXP_VARIABLE_STRING) {
				type |= SEXPT_STRING;
			} else if (Sexp_variables[sexp_var_index].type & SEXP_VARIABLE_NUMBER) {
				type |= SEXPT_NUMBER;
			} else {
				Int3();
			}

			char node_text[2 * TOKEN_LENGTH + 2];
			sprintf(node_text, "%s(%s)", item.text.c_str(), Sexp_variables[sexp_var_index].text);
			add_variable_data(node_text, type);
		}
		else if (item.type & SEXPT_CONTAINER_NAME) {
			Assertion(is_container_name_opf_type(op_type) || op_type == OPF_DATA_OR_STR_CONTAINER,
				"Attempt to add default container name for a node of non-container type (%d). Please report!",
				op_type);
			add_container_name(item.text.c_str());
		}
			// modify-variable data type depends on type of variable being modified
			// (we know this block is handling the second argument since it's not OPF_VARIABLE_NAME)
		else if (Operators[op_index].value == OP_MODIFY_VARIABLE) {
			// the the variable name
			char buf2[256];
			Assert(argnum == 1);
			sexp_list_item temp_item;
			get_default_value(&temp_item, buf2, op_index, 0);
			int sexp_var_index = get_index_sexp_variable_name(temp_item.text);
			Assert(sexp_var_index != -1);

			// from name get type
			int temp_type = Sexp_variables[sexp_var_index].type;
			int type = 0;
			if (temp_type & SEXP_VARIABLE_NUMBER) {
				type = SEXPT_VALID | SEXPT_NUMBER;
			} else if (temp_type & SEXP_VARIABLE_STRING) {
				type = SEXPT_VALID | SEXPT_STRING;
			} else {
				Int3();
			}
			add_data(item.text.c_str(), type);
		}
			// all other sexps and parameters
		else {
			add_data(item.text.c_str(), item.type);
		}
	}

	return 0;
}

int sexp_tree::get_default_value(sexp_list_item* item, char* text_buf, int op, int i) {
	const char* str = NULL;
	int type, index;
	sexp_list_item* list;

	index = item_index;
	type = query_operator_argument_type(op, i);
	switch (type) {
	case OPF_NULL:
		item->set_op(OP_NOP);
		return 0;

	case OPF_BOOL:
		item->set_op(OP_TRUE);
		return 0;

	case OPF_ANYTHING:
		if (Operators[op].value == OP_INVALIDATE_ARGUMENT || Operators[op].value == OP_VALIDATE_ARGUMENT)
			item->set_data(SEXP_ARGUMENT_STRING);	// this is almost always what you want for these sexps
		else
			item->set_data("<any data>");
		return 0;

	case OPF_DATA_OR_STR_CONTAINER:
		item->set_data("<any data or string container>");
		return 0;

	case OPF_NUMBER:
	case OPF_POSITIVE:
	case OPF_AMBIGUOUS:
		// if the top level operators is an AI goal, and we are adding the last number required,
		// assume that this number is a priority and make it 89 instead of 1.
		if ((query_operator_return_type(op) == OPR_AI_GOAL) && (i == (Operators[op].min - 1))) {
			item->set_data("89", (SEXPT_NUMBER | SEXPT_VALID));
		} else if (((Operators[op].value == OP_HAS_DOCKED_DELAY) || (Operators[op].value == OP_HAS_UNDOCKED_DELAY)
			|| (Operators[op].value == OP_TIME_DOCKED) || (Operators[op].value == OP_TIME_UNDOCKED)) && (i == 2)) {
			item->set_data("1", (SEXPT_NUMBER | SEXPT_VALID));
		} else if ((Operators[op].value == OP_SHIP_TYPE_DESTROYED) || (Operators[op].value == OP_GOOD_SECONDARY_TIME)) {
			item->set_data("100", (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_SET_SUPPORT_SHIP) {
			item->set_data("-1", (SEXPT_NUMBER | SEXPT_VALID));
		} else if (((Operators[op].value == OP_SHIP_TAG) && (i == 1))
			|| ((Operators[op].value == OP_TRIGGER_SUBMODEL_ANIMATION) && (i == 3))) {
			item->set_data("1", (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_EXPLOSION_EFFECT) {
			int temp;
			char sexp_str_token[TOKEN_LENGTH];

			switch (i) {
			case 3:
				temp = 10;
				break;
			case 4:
				temp = 10;
				break;
			case 5:
				temp = 100;
				break;
			case 6:
				temp = 10;
				break;
			case 7:
				temp = 100;
				break;
			case 11:
				temp = (int) EMP_DEFAULT_INTENSITY;
				break;
			case 12:
				temp = (int) EMP_DEFAULT_TIME;
				break;
			default:
				temp = 0;
				break;
			}

			sprintf(sexp_str_token, "%d", temp);
			item->set_data(sexp_str_token, (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_WARP_EFFECT) {
			int temp;
			char sexp_str_token[TOKEN_LENGTH];

			switch (i) {
			case 6:
				temp = 100;
				break;
			case 7:
				temp = 10;
				break;
			default:
				temp = 0;
				break;
			}

			sprintf(sexp_str_token, "%d", temp);
			item->set_data(sexp_str_token, (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_CHANGE_BACKGROUND) {
			item->set_data("1", (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_ADD_BACKGROUND_BITMAP || Operators[op].value == OP_ADD_BACKGROUND_BITMAP_NEW) {
			int temp = 0;
			char sexp_str_token[TOKEN_LENGTH];

			switch (i) {
			case 4:
			case 5:
				temp = 100;
				break;

			case 6:
			case 7:
				temp = 1;
				break;
			}

			sprintf(sexp_str_token, "%d", temp);
			item->set_data(sexp_str_token, (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_ADD_SUN_BITMAP || Operators[op].value == OP_ADD_SUN_BITMAP_NEW) {
			int temp = 0;
			char sexp_str_token[TOKEN_LENGTH];

			if (i == 4) {
				temp = 100;
			}

			sprintf(sexp_str_token, "%d", temp);
			item->set_data(sexp_str_token, (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_MISSION_SET_NEBULA) {
			if (i == 0) {
				item->set_data("1", (SEXPT_NUMBER | SEXPT_VALID));
			} else {
				item->set_data("3000", (SEXPT_NUMBER | SEXPT_VALID));
			}
		} else if (Operators[op].value == OP_MODIFY_VARIABLE) {
			if (get_modify_variable_type(index) == OPF_NUMBER) {
				item->set_data("0", (SEXPT_NUMBER | SEXPT_VALID));
			} else {
				item->set_data("<any data>", (SEXPT_STRING | SEXPT_VALID));
			}
		} else if (Operators[op].value == OP_MODIFY_VARIABLE_XSTR) {
			if (i == 1) {
				item->set_data("<any data>", (SEXPT_STRING | SEXPT_VALID));
			} else {
				item->set_data("-1", (SEXPT_NUMBER | SEXPT_VALID));
			}
		} else if (Operators[op].value == OP_SET_VARIABLE_BY_INDEX) {
			if (i == 0) {
				item->set_data("0", (SEXPT_NUMBER | SEXPT_VALID));
			} else {
				item->set_data("<any data>", (SEXPT_STRING | SEXPT_VALID));
			}
		} else if (Operators[op].value == OP_JETTISON_CARGO_NEW) {
			item->set_data("25", (SEXPT_NUMBER | SEXPT_VALID));
		} else if (Operators[op].value == OP_TECH_ADD_INTEL_XSTR || Operators[op].value == OP_TECH_REMOVE_INTEL_XSTR) {
			item->set_data("-1", (SEXPT_NUMBER | SEXPT_VALID));
		} else {
			item->set_data("0", (SEXPT_NUMBER | SEXPT_VALID));
		}

		return 0;

		// Goober5000 - special cases that used to be numbers but are now hybrids
		case OPF_GAME_SND:
		{
			gamesnd_id sound_index;

			if (Operators[op].value == OP_EXPLOSION_EFFECT)
			{
				sound_index = GameSounds::SHIP_EXPLODE_1;
			}
			else if (Operators[op].value == OP_WARP_EFFECT)
			{
				sound_index = (i == 8) ? GameSounds::CAPITAL_WARP_IN : GameSounds::CAPITAL_WARP_OUT;
			}

			if (sound_index.isValid())
			{
				game_snd* snd = gamesnd_get_game_sound(sound_index);
				if (can_construe_as_integer(snd->name.c_str()))
					item->set_data(snd->name.c_str(), (SEXPT_NUMBER | SEXPT_VALID));
				else
					item->set_data(snd->name.c_str(), (SEXPT_STRING | SEXPT_VALID));
				return 0;
			}

			// if no hardcoded default, just use the listing default
			break;
		}

		// Goober5000 - ditto
		case OPF_FIREBALL:
		{
			int fireball_index = -1;

			if (Operators[op].value == OP_EXPLOSION_EFFECT)
			{
				fireball_index = FIREBALL_MEDIUM_EXPLOSION;
			}
			else if (Operators[op].value == OP_WARP_EFFECT)
			{
				fireball_index = FIREBALL_WARP;
			}

			if (fireball_index >= 0)
			{
				char *unique_id = Fireball_info[fireball_index].unique_id;
				if (strlen(unique_id) > 0)
					item->set_data(unique_id, (SEXPT_STRING | SEXPT_VALID));
				else
				{
					char num_str[NAME_LENGTH];
					sprintf(num_str, "%d", fireball_index);
					item->set_data(num_str, (SEXPT_NUMBER | SEXPT_VALID));
				}
				return 0;
			}

			// if no hardcoded default, just use the listing default
			break;
		}

		// new default value
		case OPF_PRIORITY:
			item->set_data("Normal", (SEXPT_STRING | SEXPT_VALID));
			return 0;
	}

	list = get_listing_opf(type, index, i);

	// Goober5000 - the way this is done is really stupid, so stupid hacks are needed to deal with it
	// this particular hack is necessary because the argument string should never be a default
	if (list && list->text == SEXP_ARGUMENT_STRING) {
		sexp_list_item* first_ptr;

		first_ptr = list;
		list = list->next;

		delete first_ptr;
	}

	if (list) {
		// copy the information from the list to the passed-in item
		*item = *list;

		// but use the provided text buffer
		strcpy(text_buf, list->text.c_str());
		item->text = text_buf;

		// get rid of the list, since we're done with it
		list->destroy();
		item->next = NULL;

		return 0;
	}

	// catch anything that doesn't have a default value.  Just describe what should be here instead
	switch (type) {
	case OPF_SHIP:
	case OPF_SHIP_NOT_PLAYER:
	case OPF_SHIP_POINT:
	case OPF_SHIP_WING:
	case OPF_SHIP_WING_WHOLETEAM:
	case OPF_SHIP_WING_SHIPONTEAM_POINT:
	case OPF_SHIP_WING_POINT:
		str = "<name of ship here>";
		break;

	case OPF_ORDER_RECIPIENT:
		str = "<all fighters>";
		break;

	case OPF_SHIP_OR_NONE:
	case OPF_SUBSYSTEM_OR_NONE:
	case OPF_SHIP_WING_POINT_OR_NONE:
		str = SEXP_NONE_STRING;
		break;

	case OPF_WING:
		str = "<name of wing here>";
		break;

	case OPF_DOCKER_POINT:
		str = "<docker point>";
		break;

	case OPF_DOCKEE_POINT:
		str = "<dockee point>";
		break;

	case OPF_SUBSYSTEM:
	case OPF_AWACS_SUBSYSTEM:
	case OPF_ROTATING_SUBSYSTEM:
	case OPF_TRANSLATING_SUBSYSTEM:
	case OPF_SUBSYS_OR_GENERIC:
		str = "<name of subsystem>";
		break;

	case OPF_SUBSYSTEM_TYPE:
		str = Subsystem_types[SUBSYSTEM_NONE];
		break;

	case OPF_POINT:
		str = "<waypoint>";
		break;

	case OPF_MESSAGE:
		str = "<Message>";
		break;

	case OPF_WHO_FROM:
		//str = "<any allied>";
		str = "<any wingman>";
		break;

	case OPF_WAYPOINT_PATH:
		str = "<waypoint path>";
		break;

	case OPF_MISSION_NAME:
		str = "<mission name>";
		break;

	case OPF_GOAL_NAME:
		str = "<goal name>";
		break;

	case OPF_SHIP_TYPE:
		str = "<ship type here>";
		break;

	case OPF_EVENT_NAME:
		str = "<event name>";
		break;

	case OPF_HUGE_WEAPON:
		str = "<huge weapon type>";
		break;

	case OPF_JUMP_NODE_NAME:
		str = "<Jump node name>";
		break;

	case OPF_NAV_POINT:
		str = "<Nav 1>";
		break;

	case OPF_ANYTHING:
		str = "<any data>";
		break;

	case OPF_DATA_OR_STR_CONTAINER:
		str = "<any data or string container>";
		break;

	case OPF_PERSONA:
		str = "<persona name>";
		break;

	case OPF_FONT:
		str = const_cast<char*>(font::FontManager::getFont(0)->getName().c_str());
		break;

	case OPF_AUDIO_VOLUME_OPTION:
		str = "Music";
		break;

	case OPF_POST_EFFECT:
		str = "<Effect Name>";
		break;

	case OPF_CUSTOM_HUD_GAUGE:
		str = "<Custom hud gauge>";
		break;

	case OPF_ANY_HUD_GAUGE:
		str = "<Custom or builtin hud gauge>";
		break;

	case OPF_ANIMATION_NAME:
		str = "<Animation trigger name>";
		break;

	case OPF_CONTAINER_VALUE:
		str = "<container value>";
		break;

	case OPF_MESSAGE_TYPE:
		str = Builtin_messages[0].name;
		break;

	default:
		str = "<new default required!>";
		break;
	}

	item->set_data(str, (SEXPT_STRING | SEXPT_VALID));
	return 0;
}

int sexp_tree::query_default_argument_available(int op) {
	int i;

	Assert(op >= 0);
	for (i = 0; i < Operators[op].min; i++) {
		if (!query_default_argument_available(op, i)) {
			return 0;
		}
	}

	return 1;
}

int sexp_tree::query_default_argument_available(int op, int i) {
	int j, type;
	object* ptr;

	type = query_operator_argument_type(op, i);
	switch (type) {
	case OPF_NONE:
	case OPF_NULL:
	case OPF_BOOL:
	case OPF_NUMBER:
	case OPF_POSITIVE:
	case OPF_IFF:
	case OPF_AI_CLASS:
	case OPF_WHO_FROM:
	case OPF_PRIORITY:
	case OPF_SHIP_TYPE:
	case OPF_SUBSYSTEM:
	case OPF_AWACS_SUBSYSTEM:
	case OPF_ROTATING_SUBSYSTEM:
	case OPF_TRANSLATING_SUBSYSTEM:
	case OPF_SUBSYSTEM_TYPE:
	case OPF_DOCKER_POINT:
	case OPF_DOCKEE_POINT:
	case OPF_AI_GOAL:
	case OPF_KEYPRESS:
	case OPF_AI_ORDER:
	case OPF_SKILL_LEVEL:
	case OPF_MEDAL_NAME:
	case OPF_WEAPON_NAME:
	case OPF_INTEL_NAME:
	case OPF_SHIP_CLASS_NAME:
	case OPF_HUGE_WEAPON:
	case OPF_JUMP_NODE_NAME:
	case OPF_AMBIGUOUS:
	case OPF_CARGO:
	case OPF_ARRIVAL_LOCATION:
	case OPF_DEPARTURE_LOCATION:
	case OPF_ARRIVAL_ANCHOR_ALL:
	case OPF_SUPPORT_SHIP_CLASS:
	case OPF_SHIP_WITH_BAY:
	case OPF_SOUNDTRACK_NAME:
	case OPF_STRING:
	case OPF_FLEXIBLE_ARGUMENT:
	case OPF_ANYTHING:
	case OPF_DATA_OR_STR_CONTAINER:
	case OPF_SKYBOX_MODEL_NAME:
	case OPF_SKYBOX_FLAGS:
	case OPF_SHIP_OR_NONE:
	case OPF_SUBSYSTEM_OR_NONE:
	case OPF_SHIP_WING_POINT_OR_NONE:
	case OPF_SUBSYS_OR_GENERIC:
	case OPF_BACKGROUND_BITMAP:
	case OPF_SUN_BITMAP:
	case OPF_NEBULA_STORM_TYPE:
	case OPF_NEBULA_POOF:
	case OPF_TURRET_TARGET_ORDER:
	case OPF_TURRET_TYPE:
	case OPF_POST_EFFECT:
	case OPF_TARGET_PRIORITIES:
	case OPF_ARMOR_TYPE:
	case OPF_DAMAGE_TYPE:
	case OPF_FONT:
	case OPF_HUD_ELEMENT:
	case OPF_SOUND_ENVIRONMENT:
	case OPF_SOUND_ENVIRONMENT_OPTION:
	case OPF_EXPLOSION_OPTION:
	case OPF_AUDIO_VOLUME_OPTION:
	case OPF_WEAPON_BANK_NUMBER:
	case OPF_MESSAGE_OR_STRING:
	case OPF_BUILTIN_HUD_GAUGE:
	case OPF_CUSTOM_HUD_GAUGE:
	case OPF_ANY_HUD_GAUGE:
	case OPF_SHIP_EFFECT:
	case OPF_ANIMATION_TYPE:
	case OPF_SHIP_FLAG:
	case OPF_WING_FLAG:
	case OPF_NEBULA_PATTERN:
	case OPF_NAV_POINT:
	case OPF_TEAM_COLOR:
	case OPF_GAME_SND:
	case OPF_FIREBALL:
	case OPF_SPECIES:
	case OPF_LANGUAGE:
	case OPF_FUNCTIONAL_WHEN_EVAL_TYPE:
	case OPF_ANIMATION_NAME:	
	case OPF_CONTAINER_VALUE:
	case OPF_WING_FORMATION:
	case OPF_CHILD_LUA_ENUM:
	case OPF_MESSAGE_TYPE:
		return 1;

	case OPF_SHIP:
	case OPF_SHIP_WING:
	case OPF_SHIP_POINT:
	case OPF_SHIP_WING_POINT:
	case OPF_SHIP_WING_WHOLETEAM:
	case OPF_SHIP_WING_SHIPONTEAM_POINT:
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (ptr->type == OBJ_SHIP || ptr->type == OBJ_START) {
				return 1;
			}

			ptr = GET_NEXT(ptr);
		}

		return 0;

	case OPF_SHIP_NOT_PLAYER:
	case OPF_ORDER_RECIPIENT:
		ptr = GET_FIRST(&obj_used_list);
		while (ptr != END_OF_LIST(&obj_used_list)) {
			if (ptr->type == OBJ_SHIP) {
				return 1;
			}

			ptr = GET_NEXT(ptr);
		}

		return 0;

	case OPF_WING:
		for (j = 0; j < MAX_WINGS; j++) {
			if (Wings[j].wave_count) {
				return 1;
			}
		}

		return 0;

	case OPF_PERSONA:
		return Personas.empty() ? 0 : 1;

	case OPF_POINT:
	case OPF_WAYPOINT_PATH:
		return Waypoint_lists.empty() ? 0 : 1;

	case OPF_MISSION_NAME:
		return _interface->hasDefaultMissionName() ? 1 : 0;

		// The following code is kept for when the campaign editor is implemented
		/*
		if (m_mode != MODE_CAMPAIGN) {
			if (!(*Mission_filename))
				return 0;

			return 1;
		}

		if (Campaign.num_missions > 0)
			return 1;

		return 0;
		 */

	case OPF_GOAL_NAME: {
		return _interface->hasDefaultGoal(Operators[op].value) ? 1 : 0;

		// The original code is kept until the campaign editor is implemented
		/*
		int value;

		value = Operators[op].value;

		if (m_mode == MODE_CAMPAIGN) {
			return 1;

			// need to be sure that previous-goal functions are available.  (i.e. we are providing a default argument for them)
		} else if ((value == OP_PREVIOUS_GOAL_TRUE) || (value == OP_PREVIOUS_GOAL_FALSE)
			|| (value == OP_PREVIOUS_GOAL_INCOMPLETE) || !Mission_goals.empty()) {
				return 1;
		}

		return 0;
		 */
	}

	case OPF_EVENT_NAME: {
		return _interface->hasDefaultEvent(Operators[op].value) ? 1 : 0;

		// The original code is kept until the campaign editor is implemented
		/*
		int value;

		value = Operators[op].value;
		if (m_mode == MODE_CAMPAIGN) {
			return 1;

			// need to be sure that previous-event functions are available.  (i.e. we are providing a default argument for them)
		} else if ((value == OP_PREVIOUS_EVENT_TRUE) || (value == OP_PREVIOUS_EVENT_FALSE)
			|| (value == OP_PREVIOUS_EVENT_INCOMPLETE) || !Mission_events.empty()) {
			return 1;
		}

		return 0;
		 */
	}

	case OPF_MESSAGE:
		return _interface->hasDefaultMessageParamter() ? 1 : 0;

	case OPF_VARIABLE_NAME:
		return (sexp_variable_count() > 0) ? 1 : 0;

	case OPF_SSM_CLASS:
		return Ssm_info.empty() ? 0 : 1;

	case OPF_MISSION_MOOD:
		return Builtin_moods.empty() ? 0 : 1;

	case OPF_CONTAINER_NAME:
		return get_all_sexp_containers().empty() ? 0 : 1;

	case OPF_LIST_CONTAINER_NAME:
		for (const auto& container : get_all_sexp_containers()) {
			if (container.is_list()) {
				return 1;
			}
		}
		return 0;

	case OPF_MAP_CONTAINER_NAME:
		for (const auto& container : get_all_sexp_containers()) {
			if (container.is_map()) {
				return 1;
			}
		}
		return 0;

	case OPF_MOTION_DEBRIS:
		if (Motion_debris_info.size() > 0) {
			return 1;
		}
		return 0;

	case OPF_BOLT_TYPE:
		if (Bolt_types.size() > 0) {
			return 1;
		}
		return 0;

	case OPF_ASTEROID_TYPES:
		if (!get_list_valid_asteroid_subtypes().empty()) {
			return 1;
		}
		return 0;

	case OPF_DEBRIS_TYPES:
		for (const auto& this_asteroid : Asteroid_info) {
			if (this_asteroid.type == ASTEROID_TYPE_DEBRIS) {
				return 1;
			}
		}
		return 0;

	case OPF_TRAITOR_OVERRIDE:
		return Traitor_overrides.empty() ? 0 : 1;

	case OPF_LUA_GENERAL_ORDER:
		return (ai_lua_get_num_general_orders() > 0) ? 1 : 0;

	case OPF_MISSION_CUSTOM_STRING:
		return The_mission.custom_strings.empty() ? 0 : 1;
		break;

	default:
		if (!Dynamic_enums.empty()) {
			if ((type - First_available_opf_id) < (int)Dynamic_enums.size()) {
				return 1;
			} else {
				UNREACHABLE("Unhandled SEXP argument type!");
			}
		} else {
			UNREACHABLE("Unhandled SEXP argument type!");
		}

	}

	return 0;
}


///////////////////////////////////////////// 
// ITEMS!!
///

// initialize node, type operator
//
void sexp_list_item::set_op(int op_num) {
	int i;

	if (op_num >= FIRST_OP) {  // do we have an op value instead of an op number (index)?
		for (i = 0; i < (int) Operators.size(); i++) {
			if (op_num == Operators[i].value) {
				op_num = i;
			}
		}  // convert op value to op number
	}

	op = op_num;
	text = Operators[op].text;
	type = (SEXPT_OPERATOR | SEXPT_VALID);
}


// initialize node, type data
// Defaults: t = SEXPT_STRING
//
void sexp_list_item::set_data(const char* str, int t) {
	op = -1;
	text = str;
	type = t;
}

// add a node to end of list
//
void sexp_list_item::add_op(int op_num) {
	sexp_list_item* item, * ptr;

	item = new sexp_list_item;
	ptr = this;
	while (ptr->next) {
		ptr = ptr->next;
	}

	ptr->next = item;
	item->set_op(op_num);
}

void sexp_list_item::add_data(const char* str, int t) {
	sexp_list_item* item, * ptr;

	item = new sexp_list_item;
	ptr = this;
	while (ptr->next) {
		ptr = ptr->next;
	}

	ptr->next = item;
	item->set_data(str, t);
}

// add an sexp list to end of another list (join lists)
//
void sexp_list_item::add_list(sexp_list_item* list) {
	sexp_list_item* ptr;

	ptr = this;
	while (ptr->next) {
		ptr = ptr->next;
	}

	ptr->next = list;
}

// free all nodes of list
//
void sexp_list_item::destroy() {
	sexp_list_item* ptr, * ptr2;

	ptr = this;
	while (ptr) {
		ptr2 = ptr->next;

		delete ptr;
		ptr = ptr2;
	}
}


