
#include "parse/parselo.h"


class backend_sexp_tree_item {
 	int type;
	int parent;    // pointer to parent of this item
	int child;    // pointer to first child of this item
	int next;    // pointer to next sibling
	int flags;	
	char text[2 * TOKEN_LENGTH + 2];   
};

class backend_sexp_list_item {
	int lol;
};

struct sexp_model {
    

    private:

	SCP_vector<backend_sexp_tree_item> _tree_nodes;
    SCP_vector<backend_sexp_list_item> _item_nodes;

	void free_node(int node, bool cascade = false);

};

enum class sexp_item_type{
	OPERATOR,
	VARIABLE,
	CONTAINER_NAME,
	CONTAINER_DATA,
	EDITABLE_OTHER,
	ERROR
}