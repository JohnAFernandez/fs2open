#include "globalincs/systemvars.h"
#include "globalincs/pstypes.h"
#include "parse/parse_items.h"

void parse_item::reset_fields()
{
	_table_string   = "FIX ME!";
	_description    = "Someone didn't initialize me correctly. Go get a coder.";

	_type   = PI_Settings::Item_Types::USAGE_NOTE;

	_required		= false;
	_minimum_count = NO_ENFORCED_COUNT;
	_maximum_count = NO_ENFORCED_COUNT;
	_major_version_number_implemented = INVALID_VERSION;
	_minor_version_number_implemented = INVALID_VERSION;
	_revision_number_implemented	  = INVALID_VERSION;
	_major_version_number_deprecated  = INVALID_VERSION;
	_minor_version_number_deprecated  = INVALID_VERSION;
	_revision_number_deprecated		  = INVALID_VERSION;
	_deprecation_message			  = "";
	_parent_entry = "";

	_parse_flags.reset(); 
	_entry_type = PI_Settings::Entry_Types::NONE;

	_checks.clear(); 
	_tags.clear(); 
    _data_target = nullptr;

}

SCP_string parse_item::get_table_string()
{
	return _table_string;
}

SCP_string parse_item::get_description()
{
	return _description;
}

PI_Settings::Item_Types parse_item::get_type()
{
	return _type;
}

PI_Settings::Entry_Types parse_item::get_entry_type()
{
	return _entry_type;
}


bool parse_item::required() 
{
	return _required;
}

int parse_item::get_minimum_count()
{
	return _minimum_count;
}

int parse_item::get_maximum_count()
{
	return _maximum_count;
}

int parse_item::get_major_version_number_implemented()
{
	return _major_version_number_implemented;
}

int parse_item::get_minor_version_implemented()
{
	return _minor_version_number_implemented;
}

int parse_item::get_revision_number_implemented()
{
	return _revision_number_implemented;
}

int parse_item::get_major_version_deprecated()
{
	return _major_version_number_deprecated;
}

int parse_item::get_minor_version_deprecated()
{
	return _minor_version_number_deprecated;
}

int parse_item::get_revision_number_deprecated()
{
	return _revision_number_deprecated;
}

SCP_string parse_item::get_deprecation_message()
{
	return _deprecation_message;
}

// ====== Factory functions ======
parse_item_builder* parse_item_builder::new_parse_item(SCP_string name)
{
	_product.reset_fields();
	_product._table_string = std::move(name);
	return this;
}

parse_item_builder* parse_item_builder::set_description(SCP_string description)
{
	_product._description = std::move(description);
	return this;
}

parse_item_builder* parse_item_builder::set_type(PI_Settings::Item_Types type)
{
	_product._type = type;
	return this;
}

parse_item_builder* parse_item_builder::set_required(bool required)
{
	_product._required = required;
	return this;
}

parse_item_builder* parse_item_builder::set_minimum_count(int minimum_count)
{
	_product._minimum_count = minimum_count;
	return this;
}

parse_item_builder* parse_item_builder::set_maximum_count(int maximum_count)
{
	_product._maximum_count = maximum_count;
	return this;
}

parse_item_builder* parse_item_builder::set_implementation_version(int major_implementation_version, int minor_implementation_version, int revision_implementation_number)
{
	_product._major_version_number_implemented = major_implementation_version;
	_product._minor_version_number_implemented = minor_implementation_version;
	_product._revision_number_implemented = revision_implementation_number;

	return this;
}

parse_item_builder* parse_item_builder::set_deprecation_version(int major_deprecation_version, int minor_deprecation_version, int revision_deprecation_number)
{
	_product._major_version_number_deprecated = major_deprecation_version;
	_product._minor_version_number_deprecated = minor_deprecation_version;
	_product._revision_number_deprecated = revision_deprecation_number;
	return this;
}

parse_item_builder* parse_item_builder::set_deprecation_message(SCP_string deprecation_message)
{
	_product._deprecation_message = std::move(deprecation_message);
	return this;
}

parse_item_builder* parse_item_builder::set_parent_entry(SCP_string parent_entry)
{
	_product._parent_entry = parent_entry;
	return this;
}

parse_item_builder* parse_item_builder::set_special_parse_behavior(PI_Settings::Item_Flags behavior) 
{
	_product._parse_flags.set(behavior);
	return this;
}

parse_item_builder* parse_item_builder::add_check(PI_Settings::Exception_Types check, float in)
{
	_product._checks.emplace_back(check, in);
	return this;
}

parse_item_builder* parse_item_builder::add_tag(SCP_string tag)
{
	_product._tags.push_back(tag);
	return this;
}

parse_item_builder* parse_item_builder::set_entry_type(PI_Settings::Entry_Types type_in)
{
	_product._entry_type = type_in;
	return this;
}

// if we ever switch to c++17
parse_item_builder* parse_item_builder::set_data_target(agnostic_ptr destination)
{
    _product._data_target = destination;
    return this;
}

SCP_string get_parse_item_type_description_from_type(PI_Settings::Item_Types type_in) {
	switch (type_in) {
		case PI_Settings::Item_Types::USAGE_NOTE:
			return "Help item that only exists in documentation.";
			break;
		case PI_Settings::Item_Types::STRING:
			return "Regular string";
			break;
		case PI_Settings::Item_Types::XSTR:
			return "Translateable \"XSTR\" string";
			break;
		case PI_Settings::Item_Types::INT:
			return "Integer";
			break;
		case PI_Settings::Item_Types::INT_LIST:
			return "List of Integers";
			break;
		case PI_Settings::Item_Types::LONG:
			return "Long";
			break;
		case PI_Settings::Item_Types::FLOAT:
			return "Float";
			break;
		case PI_Settings::Item_Types::FLOAT_LIST:
			return "List of floats";
			break;
		case PI_Settings::Item_Types::DOUBLE:
			return "Double";
			break;
		case PI_Settings::Item_Types::VEC3:
			return "3 Dimensional Vector";
			break;
		case PI_Settings::Item_Types::BOOL:
			return "Boolean";
			break;
		case PI_Settings::Item_Types::FLAGS:
			return "List of Flags";
			break;
		case PI_Settings::Item_Types::MARKER:
			return "Only marks a new section or provides instructions for the parser.";
			break;
		case PI_Settings::Item_Types::MODEL_FILENAME:
			return "Model filname";
			break;
		case PI_Settings::Item_Types::TEXTURE_FILENAME:
			return "Texture filename";
			break;
		case PI_Settings::Item_Types::SOUND_FILENAME:
			return "Sound filename";
			break;
		case PI_Settings::Item_Types::DAMAGE_TYPE_INDEX:
			return "Damage type";
		default:
			UNREACHABLE("A coder passed an inavlid type value to get_parse_item_type_description_from_type() of %d. Please report!", (int)type_in);
			return "";
	}
}
