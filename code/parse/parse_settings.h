#pragma once

#include "globalincs/systemvars.h"
#include "globalincs/pstypes.h"
#include "globalincs/flagset.h"

extern bool Output_parse_items;

// Because semicolons are the comment character, they will not appear in ship names.
constexpr char INTERNAL_SUBSECTION_DELIMITER = ';'; 
const SCP_string ANONYMOUS_SUBSECTION_IDENTIFIER = "ANON";

namespace PI_Settings {
/////////////////////////////////////////////////////////////////
// Parse Item Types
// What type of items we can list in our code. 
// MARKER and USAGE_NOTE do not actually mark data
enum class Item_Types
{
	USAGE_NOTE,
	STRING,
	XSTR,		// TODO IMPLEMENT
	INT,
	INT_LIST,	// TODO IMPLEMENT
	LONG,
	FLOAT,
	FLOAT_LIST, // TODO IMPLEMENT
	DOUBLE,
	VEC3,
	VEC3_LIST, // TODO IMPLEMENT
	BOOL,
	FLAGS,		// TODO IMPLEMENT
	MARKER,		
	MODEL_FILENAME,
	TEXTURE_FILENAME,
	SOUND_FILENAME,
	DAMAGE_TYPE_INDEX
};

/////////////////////////////////////////////////////////////////
// Special Parse Behaviors flag set
// this helps the parse wrapper figure out what to do in special 
// situations.
FLAG_LIST(Item_Flags)
{
	ENTRY_IDENTIFIER = 0,
	SUB_SECTION_HEADER,
	SUB_SECTION_IDENTIFIER,
	NEW_TYPE_IDENTIFIER, 
//	REPEATING_SUBSECTION,	// TODO Keep?
	// if one item in the subsection fails, the entire subsection must be erased
//	REPEATING_SUBSECTION_ALL_OR_NOTHING, // TODO Keep?
	// not required by the main entry but required by its subsection
	REQUIRED_IN_SUBSECTION,// TODO IMPLEMENT
	PART_OF_SUBSECTION,
//	RETURN_TO_SUBSECTION_HEADER, // When we enforced order, this was needed, but with out of order parsing, this probably does not need to be implemented
	NO_CREATE,	// TODO IMPLEMENT
	TEMPLATE,	// TODO IMPLEMENT	// remember, this is thrown away at the end, because we don't save templates as classes				
	VARIANT,	// TODO IMPLEMENT
	REMOVE,		// TODO IMPLEMENT
	REMOVE_SUBENTRY, // TODO IMPLEMENT
	ONE_LINE_ENTRY,	// TODO IMPLEMENT, looking at you, sounds.tbl
	NUM_VALUES
};

FLAG_LIST(Entry_Flags) {
	INHERITING,
	TEMPLATE,
	VARIANT,
	REMOVE,
	NUM_VALUES
};

FLAG_LIST(Data_Flags) {
	NO_CREATE,
	NUM_VALUES
};

FLAG_LIST(Table_Flags) {
	NO_CREATE,
	MODULAR_FILE,	// TODO IMPLEMENT
	IGNORE_ENTRY,	// TODO IMPLEMENT
	NUM_VALUES
};


/////////////////////////////////////////////////////////////////
// Table Types
// Currently allows a Table_manager to choose which filenames to 
// use based on what type of table it is, but will probably be
// used for other things in the future
enum class Table_Types
{
	AI,
	AI_PROFILES,
	ARMOR,
	ASTEROID,					// Implemented
	AUTOPILOT,
	COLORS,
	CONTROL_CONFIG_DEFAULTS,
	CREDITS,
	CUTSCENES,
	FIREBALL,
	FONTS,
	GAME_SETTINGS,
	GLOWPOINTS,
	HELP,
	HUD_GAUGES,
	ICONS,
	IFF_DEFS,
	LIGHTNING,
	MAINHALL,
	MEDALS,
	MESSAGES,
	MFLASH,
	MUSIC,
	NEBULA,
	OBJECT_TYPES,
	PIXELS,
	POST_PROCESSING,
	RANK,
	SCRIPTING,
	SHIPS,
	SEXPS,
	SOUNDS,
	SPECIES_DEFS,
	SPECIES,
	SSM,
	STARS,
	STRINGS,
	TIPS,
	TRAITOR,
	TSTRINGS,
	WEAPONS_EXPL,
	WEAPONS
};

/////////////////////////////////////////////////////////////////
// EXCEPTIONS TYPES
// two enums for listing exception types and their consequences
enum class Exception_Types {
	MINIMUM_INCLUSIVE,
	MAXIMUM_INCLUSIVE,
	MINIMUM_EXCLUSIVE,
	MAXIMUM_EXCLUSIVE,
	STRING_LENGTH_MINIMUM,
	STRING_LENGTH_MAXIMUM,
	ILLEGAL_VALUE
};

// what to do when an test fails!
enum class Exception_Results {
	REMOVE_WHOLE_ENTRY,
	REMOVE_SUBENTRY,
	GET_LAST_GOOD_REPLACEMENT
};

/////////////////////////////////////////////////////////////////
// Table Entry Types
// Another Enum to list we have to list all entry types and subtypes here (like ships and subsystems).
// Yes, I know, it's a lot, but this is so that we can easily look up the data later.  
enum class Entry_Types { // TODO: Implement Destination code
	NONE,
	ASTEROID,			// regular random asteroids 
	ASTEROID_DEBRIS		//	debris that acts like an asteroid.
};

} // namespace Parse_Items
