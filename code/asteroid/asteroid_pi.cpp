#include "asteroid_pi.h"

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

table_manager Asteroid_parse_items;
asteroid_info Asteroid_parse_target;
asteroid_split_info Asteroid_split_parse_target;

// asteroid table definitions
void init_asteroid_parse_items()
{
	auto factory = parse_item_builder(&Asteroid_parse_items, PI_Settings::Table_Types::ASTEROID);
	Warning(LOCATION, "Starting to parse asteroid parse items."); // temporary

	factory.new_parse_item(SCP_string("#Asteroid Types"))
		->set_description(SCP_string("The starting text for the asteroid table section."))
		->set_type(PI_Settings::Item_Types::MARKER)
		->set_special_parse_behavior(PI_Settings::Item_Flags::NEW_TYPE_IDENTIFIER)
		->set_implementation_version(2, 0 ,0)
		->set_entry_type(PI_Settings::Entry_Types::ASTEROID)
		->finalize();

	factory.new_parse_item(SCP_string("$Name:"))
		->set_description(SCP_string("Defines the name of asteroid or debris object"))
		->set_type(PI_Settings::Item_Types::STRING)
		->set_required(true)
		->set_implementation_version(2, 0, 0)
		->set_special_parse_behavior(PI_Settings::Item_Flags::ENTRY_IDENTIFIER)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.name))
		->finalize();


	factory.new_parse_item(SCP_string("$POF file1:"))
		->set_description(SCP_string("Defines the name of the model file (.pof) used for asteroid or debris model.  Debris models will only use POF file1."))
		->set_type(PI_Settings::Item_Types::MODEL_FILENAME)
		->set_implementation_version(2, 0, 0)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.pof_files[0]))
		->finalize();


	factory.new_parse_item(SCP_string("$POF file2:"))
		->set_description(SCP_string("Defines the name of the alternate asteroid or debris model of the set size. "
			"This does not work for debris chunks. The game will always only use POF file1."))
		->set_type(PI_Settings::Item_Types::MODEL_FILENAME)
		->set_implementation_version(2, 0, 0)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.pof_files[1]))
		->finalize();


	factory.new_parse_item(SCP_string("$POF file3:"))
		->set_description(SCP_string("Defines the name of the alternate asteroid model. "
			"This does not work for debris chunks. The game will always only use POF file1. "
			"If the object in question is has $POF file2: set as 'none' then this entry is not needed."))
		->set_type(PI_Settings::Item_Types::MODEL_FILENAME)
		->set_implementation_version(2, 0, 0)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.pof_files[2]))
		->finalize();

	factory.new_parse_item(SCP_string("$Detail distance:"))
		->set_description(SCP_string("Definies the distance where the change between different Levels-Of-Details (LODs) occurs, needs one for each LOD."))
		->set_type(PI_Settings::Item_Types::INT_LIST)
		->set_required(true)
		->set_implementation_version(2, 0, 0)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.detail_distance))		
		->finalize();

	factory.new_parse_item(SCP_string("$Max Speed:"))
		->set_description(SCP_string("Defines the maximum velocity of the asteroid"))
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_implementation_version(2, 0, 0)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.max_speed))
		->finalize();

	// NOTE: damage_type_idx_sav will eventually set the initial value of damage_type_idx
	factory.new_parse_item(SCP_string("$Damage Type:"))
		->set_description(SCP_string("*Defines the damage type of the asteroid explosion. Requires armor.tbl"))
		->set_implementation_version(3, 6, 10)
		->set_type(PI_Settings::Item_Types::DAMAGE_TYPE_INDEX)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.damage_type_idx_sav))
		->finalize();

	factory.new_parse_item(SCP_string("$Explosion Animations:"))
		->set_description(SCP_string("Defines the explosion animations used for the asteroid, using the index from fireball.tbl."))
		->set_implementation_version(3, 6, 15)
		->set_type(PI_Settings::Item_Types::INT_LIST)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.explosion_bitmap_anims))
		->finalize();

	factory.new_parse_item(SCP_string("$Explosion Radius Mult:"))
		->set_description(SCP_string("Defines the radius multiplier for the explosion fireballs. "
			"Setting to 1.0 causes the radius of the fireball to be equal to the radius of the asteroid. "
			"Defaults to 1.5 for large asteroids and 1.0 for others."))
		->set_implementation_version(3, 6, 15)
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.fireball_radius_multiplier))
		->finalize();

	factory.new_parse_item(SCP_string("$Expl inner rad:"))
		->set_description(SCP_string("Radius at which the full explosion damage is done, in meters."))
		->set_implementation_version(2, 0, 0)
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.inner_rad))
		->finalize();

	factory.new_parse_item(SCP_string("$Expl outer rad:"))
		->set_description(SCP_string("Maximum radius at which any damage is done, in meters."))
		->set_implementation_version(2, 0, 0)
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.outer_rad))
		->finalize();

	factory.new_parse_item(SCP_string("$Expl damage:"))
		->set_description(SCP_string("Amount of damage done inside the inner radius."))
		->set_implementation_version(2, 0, 0)
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.damage))
		->finalize();


	factory.new_parse_item(SCP_string("$Expl blast:"))
		->set_description(SCP_string("The intensity of the blast effect when you're within the outer radius."))
		->set_implementation_version(2, 0, 0)
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.blast))
		->finalize();

	factory.new_parse_item(SCP_string("$Hitpoints:"))
		->set_description(SCP_string("Defines the hitpoints of the asteroid or debris piece. "
			"Hitpoints are modified downward by skill level."))
		->set_implementation_version(2, 0, 0)
		->set_type(PI_Settings::Item_Types::FLOAT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_parse_target.initial_asteroid_strength))
		->finalize();

	factory.new_parse_item(SCP_string("$Split:"))
		->set_description(SCP_string("Defines what kind of asteroids this type of asteroid should split into when destroyed. "
			"You may define any number of $Split rules. The int references an asteroid defined by this table. "
			"The first asteroid defined by the table has index 0, the second one has index 1, etc."))
		->set_implementation_version(3, 6, 15)
		->set_type(PI_Settings::Item_Types::INT)
		->set_special_parse_behavior(PI_Settings::Item_Flags::SUB_SECTION_HEADER)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_split_parse_target.asteroid_type))
		->finalize();

	factory.new_parse_item(SCP_string("+Min:"))
		->set_description(SCP_string("The minimum number of spawned asteroids. Default 0."))
		->set_implementation_version(3, 6, 15)
		->set_type(PI_Settings::Item_Types::INT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_split_parse_target.min))
		->finalize();

	factory.new_parse_item(SCP_string("+Max:"))
		->set_description(SCP_string("The maximum number of spawned asteroids. Default 0."))
		->set_implementation_version(3, 6, 15)
		->set_type(PI_Settings::Item_Types::INT)
		->set_data_target(reinterpret_cast<agnostic_ptr>(&Asteroid_split_parse_target.max))
		->finalize();

	Warning(LOCATION, "Items parsed!  The size is... %d", static_cast<int>(Asteroid_parse_items.get_size()));

	// once all options have been added, loop through them to add them to the map to allow
	// out of order parsing.
	factory.index_search_strings();

	if (Output_parse_items){
		mprintf(("PARSE ITEMS: Outputting items from Asteroid.tbl \n"));
		Asteroid_parse_items.dump_to_file();
	}
}


