#include "mod_table/mod_table_pi.h"

/*	quick reference!!

_table_string   = "Name"				// string, name of the itme.
_description    = "Description"			// string, the description that modders will use to figure out how to use this item.
_type   = Data_Types::			// which type of input this is, uses the Data_Types enum class
_required								// bool, is this a required field?
_minimum_count 						// int, how many inputs are *required* for this field
_major_version_number_implemented   	// int, retail is 2
_minor_version_number_implemented		// int, retail is 0 
_revision_number_implemented	   		// int, retail is 0
_major_version_number_deprecated		// int, usually not defined unless this option was actually deprecated.
_minor_version_number_deprecated		// int, usually not defined unless this option was actually deprecated.
_revision_number_deprecated				// int, usually not defined unless this option was actually deprecated.
_deprecation_message					// string why was this deprecated??
*/

table_manager Mod_table_parse_items;


// asteroid table definitions
void init_mod_table_parse_items()
{
	auto factory = parse_item_builder(&Mod_table_parse_items, PI_Settings::Table_Types::GAME_SETTINGS);

	factory->set_initial_miscellaneous();

	factory->new_parse_item(SCP_string("Mod Table"))
		->set_description(SCP_string("Parse items for Mod_settings.tbl have not been complete."))
		->set_type(PI_Settings::Item_Types::STRING)
		->set_required(true)
		->set_implementation_version(2, 0, 0)
		->set_special_parse_behavior(PI_Settings::Item_Flags::ENTRY_IDENTIFIER)
		->finalize(&Mod_table_parse_items);

	factory;
}