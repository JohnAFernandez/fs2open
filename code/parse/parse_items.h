#pragma once

#include "globalincs/systemvars.h"
#include "globalincs/pstypes.h"
#include "parse/parse_settings.h"

// forward declarations for the classes defined in parse_manager.h
class parse_item_builder;
class table_manager;

typedef void* agnostic_ptr;

// Constants for default values
constexpr int NO_ENFORCED_COUNT = -1;
constexpr int INVALID_VERSION = -1;


class parse_item {
	friend class parse_item_builder;

private:
	// basic description 
	SCP_string	_table_string   = "FIX ME!";
	SCP_string	_description    = "Someone didn't initialize me correctly. Go get a coder.";

	PI_Settings::Item_Types	_type   = PI_Settings::Item_Types::USAGE_NOTE;
	// rules for this element
	bool		_required		= false;
	int			_minimum_count 	= NO_ENFORCED_COUNT;		// if this is a list that has a specific number of entries, mark it with this variable
	int			_maximum_count 	= NO_ENFORCED_COUNT;
	int			_major_version_number_implemented = INVALID_VERSION;
	int			_minor_version_number_implemented = INVALID_VERSION;
	int			_revision_number_implemented	  = INVALID_VERSION;
	int			_major_version_number_deprecated  = INVALID_VERSION;
	int			_minor_version_number_deprecated  = INVALID_VERSION;
	int			_revision_number_deprecated		  = INVALID_VERSION;
	SCP_string	_deprecation_message			  = "";

	SCP_string _parent_entry = ""; // what entry starts its subsection?
	flagset<PI_Settings::Item_Flags> _parse_flags; // Do we need to do anything special?
	PI_Settings::Entry_Types _entry_type = PI_Settings::Entry_Types::NONE;

	SCP_vector<std::pair<PI_Settings::Exception_Types, float>> _checks; 

	// At some point we want to be able to use these to allow related fields to be close to each other.
	SCP_vector<SCP_string> _tags; 

    agnostic_ptr _data_target = nullptr;


	void reset_fields();

public:
	parse_item() = default;

	SCP_string get_table_string();
	SCP_string get_description();
	PI_Settings::Item_Types get_type();
	PI_Settings::Entry_Types get_entry_type();
	bool required();
	int get_minimum_count();
	int get_maximum_count();
	int get_major_version_number_implemented();
	int get_minor_version_implemented();
	int get_revision_number_implemented();
	int get_major_version_deprecated();
	int get_minor_version_deprecated();
	int get_revision_number_deprecated();
	SCP_string get_deprecation_message();
	flagset<PI_Settings::Item_Flags>& get_parse_behaviors() { return _parse_flags; } 
	
};

class parse_item_builder {
	parse_item _product;
	table_manager* _target_table;
public:
	parse_item_builder(table_manager* target_table, PI_Settings::Table_Types type);

	parse_item_builder* new_parse_item(SCP_string name);
	parse_item_builder* set_description(SCP_string description);
	parse_item_builder* set_type(PI_Settings::Item_Types type);
	parse_item_builder* set_required(bool required);
	parse_item_builder* set_minimum_count(int minimum_count);
	parse_item_builder* set_maximum_count(int maximum_count);
	parse_item_builder* set_implementation_version(int major_implementation_version, int minor_implementation_version, int revision_implementation_number);
	parse_item_builder* set_deprecation_version(int major_deprecation_version, int minor_deprecation_version, int revision_deprecation_number);
	parse_item_builder* set_deprecation_message(SCP_string deprecation_message);
	parse_item_builder* set_parent_entry(SCP_string parent_entry);
	parse_item_builder* set_special_parse_behavior(PI_Settings::Item_Flags behavior); // Do we need to do anything special?
	parse_item_builder* add_check(PI_Settings::Exception_Types check, float in);
	parse_item_builder* add_tag(SCP_string tag);
	parse_item_builder* set_entry_type(PI_Settings::Entry_Types type_in);	
    parse_item_builder* set_data_target(agnostic_ptr destination);

	void index_search_strings();
	void finalize();

};
