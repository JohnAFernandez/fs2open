#pragma

#ifdef WIN32
#include <direct.h>
#endif

#include "globalincs/pstypes.h"
#include "globalincs/version.h"
#include "osapi/osapi.h"
#include "cfile/cfilesystem.h"
#include "parse_manager.h"
#include "asteroid/asteroid_pi.h"

extern table_manager Asteroid_parse_items; 	// temporary

parse_item_builder::parse_item_builder(table_manager* target_table, PI_Settings::Table_Types type)
{
	// This really, really should never happen, and should only happen during testing when a developer goofed
	// and the cause should be clear. So I think an assert instead of assertion is fine.
	Assert(target_table != nullptr);

	_target_table = target_table;
	_target_table->_table_type = type;

	// not set from arguments
	_target_table->_indexed = false;
}

void parse_item_builder::index_search_strings()
{
	_target_table->index_search_strings();
}

// need two functions defined here because we need to reference the target table.
void parse_item_builder::finalize()
{
	_target_table->_all_items.push_back(_product);
}

// dumps the documentation to a file. (code shamelessly ripped from outwnd.cpp and sexp.cpp)
void table_manager::dump_to_file()
{

	// Make sure the directory is created.
	_mkdir(os_get_config_path(Pathtypes[CF_TYPE_DATA].path).c_str());

	SCP_string pre_filename = get_table_filename_from_type(_table_type);

	SCP_string filename = pre_filename + ".html";

	char pathname[MAX_PATH_LEN];
	memset(pathname, 0, sizeof(pathname));
	snprintf(pathname, MAX_PATH_LEN, "%s/%s", Pathtypes[CF_TYPE_DATA].path, filename.c_str());

	SCP_string fullpath = os_get_config_path(pathname);

	FILE* file_out = fopen(fullpath.c_str(), "wb");
	
	if (file_out == nullptr) {
		Warning(LOCATION, "Unable to open the file location in order to print %s documenation", pre_filename.c_str());
		return;
	}

	//Header
	fprintf(file_out, "<html>\n<head>\n\t<title>%s Output - FSO v%s</title>\n</head>\n", pre_filename.c_str(), FS_VERSION_FULL);
	fputs("<body>", file_out);
	fprintf(file_out,"\t<h1>%s Documentation - FSO v%s</h1>\n", pre_filename.c_str(), FS_VERSION_FULL);

	//dl dt dd
	for (auto& current_item : _all_items)
	{
		fprintf(file_out, "<p>\t\t<dl> <dt> Item: </dt> <dd>%s</dd>\n", current_item.get_table_string().c_str());
		fprintf(file_out, "\t\t<dt> Description: </dt> <dd>%s</dd>\n", current_item.get_description().c_str());
		if (current_item.get_type() != PI_Settings::Item_Types::USAGE_NOTE) {
			fprintf(file_out, "\t\t<dt> Type: </dt> <dd>%s</dd>\n", get_parse_item_type_description_from_type(current_item.get_type()).c_str());
		}

		if (current_item.get_minimum_count() > NO_ENFORCED_COUNT) {
			fprintf(file_out, "\t\t<dt> Required number of entires: </dt> <dd>%d</dd>\n", current_item.get_minimum_count());
		}

		if (current_item.get_major_version_number_implemented() > INVALID_VERSION) {
			fprintf(file_out, "\t\t<dt> Version Implemented: </dt><dd>%d.%d.%d</dd>\n", current_item.get_major_version_number_implemented(), current_item.get_minor_version_implemented(), current_item.get_revision_number_implemented());
		}

		if (current_item.get_major_version_deprecated() > INVALID_VERSION) {
			fprintf(file_out, "\t\t<dt> Version Deprecated: </dt> <dd>%d.%d.%d</dd>\n", current_item.get_major_version_deprecated(), current_item.get_minor_version_deprecated(), current_item.get_revision_number_deprecated());
			fprintf(file_out, "\t\t<dt> Deprecation Reason: </dt> <dd>%s</dd>\n", current_item.get_deprecation_message().c_str());
		}
		fputs("<dl></p>", file_out);
	}

	fputs("</body>\n</html>\n", file_out);

	fclose(file_out);
}

SCP_string get_table_filename_from_type(PI_Settings::Table_Types type_in)
{
	switch (type_in) {
	case PI_Settings::Table_Types::AI:
		return "ai.tbl";
		break;
	case PI_Settings::Table_Types::AI_PROFILES:
		return "ai_profiles.tbl";
		break;
	case PI_Settings::Table_Types::ARMOR:
		return "armor.tbl";
		break;
	case PI_Settings::Table_Types::ASTEROID:
		return "asteroid.tbl";
		break;
	case PI_Settings::Table_Types::AUTOPILOT:
		return "autopilot.tbl";
		break;
	case PI_Settings::Table_Types::COLORS:
		return "colors.tbl";
		break;
	case PI_Settings::Table_Types::CONTROL_CONFIG_DEFAULTS:
		return "controlconfigdefaults.tbl";
		break;
	case PI_Settings::Table_Types::CREDITS:
		return "credits.tbl";
		break;
	case PI_Settings::Table_Types::CUTSCENES:
		return "cutscenes.tbl";
		break;
	case PI_Settings::Table_Types::FIREBALL:
		return "fireball.tbl";
		break;
	case PI_Settings::Table_Types::FONTS:
		return "fonts.tbl";
		break;
	case PI_Settings::Table_Types::GAME_SETTINGS:
		return "game_settings.tbl";
		break;
	case PI_Settings::Table_Types::GLOWPOINTS:
		return "glowpoints.tbl";
		break;
	case PI_Settings::Table_Types::HELP:
		return "help.tbl";
		break;
	case PI_Settings::Table_Types::HUD_GAUGES:
		return "hud_gauges.tbl";
		break;
	case PI_Settings::Table_Types::ICONS:
		return "icons.tbl";
		break;
	case PI_Settings::Table_Types::IFF_DEFS:
		return "iff_defs.tbl";
		break;
	case PI_Settings::Table_Types::LIGHTNING:
		return "lightning.tbl";
		break;
	case PI_Settings::Table_Types::MAINHALL:
		return "mainhall.tbl";
		break;
	case PI_Settings::Table_Types::MEDALS:
		return "medals.tbl";
		break;
	case PI_Settings::Table_Types::MESSAGES:
		return "messages.tbl";
		break;
	case PI_Settings::Table_Types::MFLASH:
		return "mflash.tbl";
		break;
	case PI_Settings::Table_Types::MUSIC:
		return "music.tbl";
		break;
	case PI_Settings::Table_Types::NEBULA:
		return "nebula.tbl";
		break;
	case PI_Settings::Table_Types::OBJECT_TYPES:
		return "objecttypes.tbl";
		break;
	case PI_Settings::Table_Types::PIXELS:
		return "pixels.tbl";
		break;
	case PI_Settings::Table_Types::POST_PROCESSING:
		return "credits.tbl";
		break;
	case PI_Settings::Table_Types::RANK:
		return "rank.tbl";
		break;
	case PI_Settings::Table_Types::SCRIPTING:
		return "scripting.tbl";
		break;
	case PI_Settings::Table_Types::SHIPS:
		return "ships.tbl";
		break;
	case PI_Settings::Table_Types::SEXPS:
		return "sexps.tbl";
		break;
	case PI_Settings::Table_Types::SOUNDS:
		return "sounds.tbl";
		break;
	case PI_Settings::Table_Types::SPECIES_DEFS:
		return "species_defs.tbl";
		break;
	case PI_Settings::Table_Types::SPECIES:
		return "species.tbl";
		break;
	case PI_Settings::Table_Types::SSM:
		return "ssm.tbl";
		break;
	case PI_Settings::Table_Types::STARS:
		return "stars.tbl";
		break;
	case PI_Settings::Table_Types::STRINGS:
		return "strings.tbl";
		break;
	case PI_Settings::Table_Types::TIPS:
		return "tips.tbl";
		break;
	case PI_Settings::Table_Types::TRAITOR:
		return "traitor.tbl";
		break;
	case PI_Settings::Table_Types::TSTRINGS:
		return "tstrings.tbl";
		break;
	case PI_Settings::Table_Types::WEAPONS_EXPL:
		return "weapon_expl.tbl";
		break;
	case PI_Settings::Table_Types::WEAPONS:
		return "weapons.tbl";
		break;
	default:
		UNREACHABLE("A coder made a mistake and passed a bad Table_Type of %d in get_table_filename_from_type, please report!", (int)type_in);
		return "";
		break;
	}
}

// add non-standard delimiters here as they are discovered.
char table_manager::_get_delimiter()
{
	switch(_table_type){
		case PI_Settings::Table_Types::STRINGS:
			return ',';
			break;

		default:
			return ':';
			break;
	}
}

void table_manager::parse_table_file() 
{
	//Warning(LOCATION, "Parsing %s",get_table_filename_from_type(this->_table_type).c_str());
	read_file_text(get_table_filename_from_type(this->_table_type).c_str(), CF_TYPE_TABLES);
	reset_parse();

	SCP_string read_line;
	const char delimiter = _get_delimiter();

	_current_line = 0;

//try{
	do {
		read_line = "";
		// use ye ol parsing functions so we don't have to reinvent the wheel
		// get to the next end of line
		copy_to_eoln(read_line, nullptr, Mp);
		// move the file content pointer to the next line
//		Warning(LOCATION, "Read %s", read_line.c_str());

		// drop the ending white space to more easily match
		drop_trailing_white_space(read_line);
//		Warning(LOCATION, "Changed to %s", read_line.c_str());

		// if we had an empty line, just keep going. 
		if (read_line.empty()) {
//			Warning(LOCATION, "Branch 1");

			// if we're at the end of the file, we need to stop now.
			if (*Mp == '\0'){
//				Warning(LOCATION, "Breaking.");

				break;
			}

			++Mp;
			
		} else {
			// don't need to deal with comments, since they are stripped earlier!
			// but we do need to deal with quoations marks


			// all table options end with a colon, so find that colon.  
			std::string::size_type delimiter_loc = read_line.find_first_of(delimiter);
//			Warning(LOCATION, "Branch 2, delimiter location is %d", static_cast<int>(delimiter_loc));

			if (delimiter_loc == std::string::npos || delimiter_loc == read_line.length()){
				// check that we already have a parse item that matches the string found
				_activate_item(read_line);
				Mp += read_line.length();
				//Warning(LOCATION, "No delimiter.");

			} else {
				SCP_string option_string = read_line.substr(0, delimiter_loc + 1);
				// Since we are only wrapping the stuff functions, we need to advance Mp to where it usually is.
				Mp += option_string.length();
				//Warning(LOCATION, "Got an option string of %s. ", option_string.c_str());
				_add_data(option_string);
			}
		}

		read_line.clear();
		_current_line++;
	} while (*Mp != '\0');
//}

// I'm not sure we're going to need this catch anymore.  We'll see.
//catch {
	
//}
	// if we get here, we're done with this file and need to move on to the next one.
}

// add the data to our aggregate.  Special behavior needs to be handled before we get here!
void table_manager::_add_data(SCP_string option)
{
	//Warning(LOCATION, "I am an add data call!");
	// sorry this is a little convoluted. Look up the index by using the 
	// _item_indices entry 
	auto index = _item_indices.find(option);

	flagset<PI_Settings::Data_Flags> empty_flagset;

	// so if we found a valid option, add the data read from the table.
	if (index != _item_indices.end()) {

		// retrieve the item.  It really should exist!
		Assert (index->second < _all_items.size());
		auto& item =  _all_items[index->second];
		
		// adjust the address to match this new entry.
		_current_entry_address.parse_option = item.get_table_string();

		switch (item.get_type()) {
			case PI_Settings::Item_Types::STRING:
			{
				SCP_string string_out;
				stuff_string(string_out, F_NAME);

				table_entry_options temp_options;

				temp_options.type = _current_entry_address.type;
				temp_options.entry_name = string_out;
				temp_options.inheritance_target = "";

				//Warning(LOCATION, "STRING %s!", out.c_str());table_data(size_t count_in, SCP_string filep_in, size_t line_in, T data_in){
				if (item.get_parse_behaviors()[PI_Settings::Item_Flags::ENTRY_IDENTIFIER]) {
					_current_entry_address.entry_name = string_out;

					// have to use find here, because it needs to be initialized to zero
					if (_entry_type_counts.find(_current_entry_address.type) != _entry_type_counts.end()) {
						++_entry_type_counts[_current_entry_address.type];
					} else {
						_entry_type_counts[_current_entry_address.type] = 0;
					}

					bool found = false;
					// use [] because we can just insert if we don't already have an item of that type
					for(auto& entry : _table_entries[_current_entry_address.type]){
						if (entry.entry_name == string_out) {
							found = true;
							break;
						}
					}

					if (!found) {
						_table_entries[_current_entry_address.type].push_back(temp_options);
					}

				} else {
					_accepted_data.strings[_current_entry_address].push_back(temp_options);
				}

				break;
			}

			case PI_Settings::Item_Types::INT:
			{
				int int_out;
				stuff_int(&int_out);
				//Warning(LOCATION, "Int %d", out);
				_accepted_data.ints[_current_entry_address].
					emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					int_out);
				break;
			}

			case PI_Settings::Item_Types::XSTR:
			{
				SCP_string xstr_out;
				Warning(LOCATION, "XSTR, No implementation!");
				// TODO: find xstr parsing function 
				_accepted_data.strings[_current_entry_address].
					emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					xstr_out);
				break;
			}					

			case PI_Settings::Item_Types::INT_LIST:
			{
				Warning(LOCATION, "Int list, no implementation!!");
				mprintf(("Int list data type table option not yet implemented.\n"));
				break;
			}
			case PI_Settings::Item_Types::LONG:
			{
				long long_out = 0;
				mprintf(("Parsing long in parse items code, but no long parsing function.  Please implement.\n"));
				_accepted_data.longs[_current_entry_address].
					emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					long_out);
				break;
			}

			case PI_Settings::Item_Types::FLOAT:
			{
				float float_out;
				stuff_float(&float_out);
				//Warning(LOCATION, "Float! %f", float_out);
				_accepted_data.floats[_current_entry_address].
				emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					float_out);
				break;
			}

			case PI_Settings::Item_Types::FLOAT_LIST:
			{
				Warning(LOCATION, "Float List! - No Implementation!");
				mprintf(("Float list data type table option not yet implemented.\n"));
				break;
			}

			case PI_Settings::Item_Types::DOUBLE:
			{
				Warning(LOCATION, "Double! - No Implementation!");
				mprintf(("Double data type table option not yet implemented.\n"));
				break;
			}

			case PI_Settings::Item_Types::VEC3:
			{
				vec3d vec_out;
				stuff_vec3d(&vec_out);
				Warning(LOCATION, "Vec3, %f %f %f", vec_out.xyz.x, vec_out.xyz.y, vec_out.xyz.z);
				_accepted_data.vec3s[_current_entry_address].
				emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					vec_out);
				break;	
			}

			case PI_Settings::Item_Types::VEC3_LIST:
			{
				Warning(LOCATION, "Vector list, no implementation!");
				mprintf(("Unimplemented parsing type. Temp message\n"));				
				break;
			}

			case PI_Settings::Item_Types::BOOL:
			{
				bool bool_out;
				stuff_boolean(&bool_out);
//				Warning(LOCATION, "Bool! %s!", (out) ? "true" : "false");
				_accepted_data.bools[_current_entry_address].
					emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					bool_out);
				break;
			}

			case PI_Settings::Item_Types::FLAGS:
			{
				Warning(LOCATION, "Flags! No Implementation!");
				mprintf(("Unimplemented parsing type. Temp message\n"));
				break;
			}

			case PI_Settings::Item_Types::MODEL_FILENAME:
			{
				SCP_string model_out;
				stuff_string(model_out, F_NAME);
				Warning(LOCATION, "Model filename! %s!", model_out.c_str());
				_accepted_data.strings[_current_entry_address].
					emplace_back(_entry_type_counts[_current_entry_address.type], 
					_current_filepath,
					_current_line,
					empty_flagset,
					model_out);
				break;
			}

			case PI_Settings::Item_Types::TEXTURE_FILENAME:
			{
				Warning(LOCATION, "Texture Filename! - No Implementation!");
				mprintf(("Unimplemented parsing type. Temp message\n"));
				break;
			}

			case PI_Settings::Item_Types::SOUND_FILENAME:
			{
				Warning(LOCATION, "Sound Filename! - No Implementation!");
				mprintf(("Unimplemented parsing type. Temp message\n"));
				break;
			}
			case PI_Settings::Item_Types::DAMAGE_TYPE_INDEX:
			{
				// Special type. Table writer places the name, later on the index is populated.

			}
			case PI_Settings::Item_Types::MARKER:
				UNREACHABLE("The parse item code is trying to use the marker subtype to insert table data.");		
			default:
				UNREACHABLE("Unknown, unsupported parse type of %d passed to the table.  This really shouldn't be happening. Please report!", item.get_type());
			break;
		}
	}
}

bool table_manager::_activate_item(SCP_string item_text) {
	auto index_iterator = _item_indices.find(item_text);

	if (index_iterator != _item_indices.end()) {
		auto& found_item = _all_items[index_iterator->second];

		auto& parse_behaviors = found_item.get_parse_behaviors();

		if (parse_behaviors[PI_Settings::Item_Flags::NEW_TYPE_IDENTIFIER]) {
			Assertion(found_item.get_entry_type() != PI_Settings::Entry_Types::NONE, "A parse item is set to be a type switcher, but does not have a new type to switch to because it is set to None! Please fix!");

			// TODO add check that this entry is not already defined as a template.

			// if we have a new type of entry, change the entry address to empty 
			// so that we don't accidentally add that entry to another info struct.
			if(_current_entry_address.type != found_item.get_entry_type()){
				_current_entry_address.type = found_item.get_entry_type();
				_current_entry_address.entry_name.clear();
				_current_entry_address.parse_option.clear();
			}
		// TODO figure out how common anonymous subsections are, and if there are any that are parallel, instead of straight down.
		} else if(parse_behaviors[PI_Settings::Item_Flags::SUB_SECTION_HEADER]) {
			auto entry_type = found_item.get_entry_type();
			Assertion(entry_type != PI_Settings::Entry_Types::NONE, "A parse item is set to be a subtype header, but does not have a new type to switch to because it is set to None! Please fix!");
			
			// this means we have a new anonymous subsection.
			if(_current_entry_address.type != found_item.get_entry_type()) {

				// we need to generate an anonymous alias to fit it in with the rest of the data
				if (_current_anon_entry_indices.find(entry_type) == _current_anon_entry_indices.end()) {
					_current_anon_entry_indices[entry_type] = 0;
				} else {
					++_current_anon_entry_indices[entry_type];
				}

			} else {
				// same as the previous anonymous subsection, so not further down the heirarchy
				_entry_heirarchy.pop_back();
				++_current_anon_entry_indices[entry_type];
			}

			// construct the new subsection name
			_entry_heirarchy.push_back(INTERNAL_SUBSECTION_DELIMITER + ANONYMOUS_SUBSECTION_IDENTIFIER + " " + found_item.get_table_string() + std::to_string(_current_anon_entry_indices[entry_type]));

			// rebuild the current entry name.
			_current_entry_address.entry_name.clear();

			for (auto& name : _entry_heirarchy) {
				_current_entry_address.entry_name.append(name);
			}

		// the rest of the behaviors *must not* happen at the same time as a new type identifier
		} else {

			if (parse_behaviors[PI_Settings::Item_Flags::NO_CREATE]) {
				// TODO add warning that templates cannot be listed with no create 
				_table_flags.set(PI_Settings::Table_Flags::NO_CREATE);
			}

			if (parse_behaviors[PI_Settings::Item_Flags::TEMPLATE]) {
				_table_entries[_current_entry.first][_current_entry.second].flags.set(PI_Settings::Entry_Flags::TEMPLATE);
			}

			if (parse_behaviors[PI_Settings::Item_Flags::REMOVE] || parse_behaviors[PI_Settings::Item_Flags::REMOVE_SUBENTRY]) {
				_table_entries[_current_entry.first][_current_entry.second].flags.set(PI_Settings::Entry_Flags::REMOVE);
			}

		}

		// TODO add piece about if it's not part of the current subsection go up the subsection tree		


	} else {
		// for now I think we're fine, but maybe an nprintf will be good in the future.
	}

	return true;
}

// this populated the _item_indices map.  
void table_manager::index_search_strings() 
{
	Warning(LOCATION, "I really am indexing...." );
	// since this is a public function and should only run once.
	// So return if already done.
	if (_indexed){
		return;
	}

	uint index = 0;

	for (auto& item : _all_items) {
		_item_indices.insert({item.get_table_string(), index});
		++index;
	}

	_indexed = true;
}

void init_parse_tables() 
{
	// Most of this is temporary test code, at the moment

	// Make sure the directory is created.
	_mkdir(os_get_config_path(Pathtypes[CF_TYPE_DATA].path).c_str());

	SCP_string pre_filename = get_table_filename_from_type(Asteroid_parse_items.get_table_type());

	SCP_string filename = pre_filename + "_tst.txt";

	char pathname[MAX_PATH_LEN];

	memset(pathname, 0, sizeof(pathname));

	snprintf(pathname, MAX_PATH_LEN, "%s/%s", Pathtypes[CF_TYPE_DATA].path, filename.c_str());

	// list the table table types here.
	init_asteroid_parse_items();
	Asteroid_parse_items.parse_table_file();

	SCP_string fullpath = os_get_config_path(pathname);

	Asteroid_parse_items.test_dump(fullpath);

}

// wouldn't an automatic checker for our inputs be nifty?? Oh wait...
void table_manager::validate_parse_items()
{
#ifndef NDEBUG
	// perform a series of checks on each item to ensure everything has been
	// entered correctly.  But only on debug.  This function is really just a way for
	// a coder to check that they didn't do anything obviously wrong.
//	for (auto& item : _all_items) {
		// TODO Fix or add to unit checks
/*		if (item._parse_flags[PI_Settings::Item_Flags::ENTRY_IDENTIFIER]){
			Assertion(item.get_type() == PI_Settings::Item_Types::STRING, "Parse item %s was marked as an entry identifier, but it was not of type string.  It was of type: %d ", item.get_table_string.c_str(), get_parse_item_type_description_from_type(item.get_type()).c_str());
		}
		PI_Settings::
		// TODO create checks for implementation version.

		// deprecation checks
		if (item.get_major_version_deprecated() != INVALID_VERSION || item.get_minor_version_deprecated() != INVALID_VERSION || item.get_revision_number_deprecated() != INVALID_VERSION) {
			Assertion(item.get_major_version_deprecated() >= INVALID_VERSION && item.get_minor_version_deprecated() > INVALID_VERSION && item.get_revision_number_deprecated() > INVALID_VERSION, 
			"Wonky deprecation version of %d.%d.%d found for parse item %s for %s.  Please fix!",item.get_major_version_deprecated(), item.get_minor_version_deprecated(), item.get_revision_number_deprecated(), item.get_table_string().c_str(), get_table_filename_from_type(_table_type));
			Assertion(!item.get_deprecation_message().empty(), "Parse item %s for %s is marked as deprecated but does not have a deprecation message. Please fix!", item.get_table_string().c_str(), get_table_filename_from_type(_table_type));
		}
*/
		// TODO create check that item that indicates a new entry type is of item type marker!

		// TODO create check that a marker that changes types is not of type none.

		// TODO create check that string types will have length and contents validity checks only.

		// TODO create check that ensures template and variant are not mix and match

		// TODO create check that if we didn't have the file delimiter, then we cannot have a one line entry

		// TODO add check that parse items that are not marker or note must have a destination set

		// TODO add check that one line entry item flag contains the table delimiter

		// TODO add check that variant item flag must have a delimter

//	}

#endif
}

void table_manager::test_dump(SCP_string path)
{
	FILE* file_out = fopen(path.c_str(), "wb");
	static bool b1 = false;
	static bool b2 = false; 
	static bool b3 = false;
	static bool b4 = false;
	static bool b5 = false;
	static bool b6 = false;

	for (auto& option : _accepted_data.strings) {
		if (!b1) {
				Warning(LOCATION, "In loop 1!");
			b1 = true;
		}
		
		for(auto& item : option.second) {
			if (!b2) {
				Warning(LOCATION, "In loop 2!");
				b2 = true;
			}
			Warning(LOCATION, "Found %s ", item.data.c_str());

		}
	}

	for (auto& option : _accepted_data.ints) {
		if (!b3) {
			Warning(LOCATION, "In loop 3!");
			b3 = true;
		}
		for(auto& item : option.second) {
		if (!b4) {
			Warning(LOCATION, "In loop 4!");
			b4 = true;
		}
				Warning(LOCATION, "Found %i ", item.data);

		}
	}

	for (auto& option : _accepted_data.floats) {
		if (!b5) {
			Warning(LOCATION, "In loop 5!");
			b5 = true;
		}
		for(auto& item : option.second) {
			if (!b6){
				Warning(LOCATION, "In loop 6!");
				b6 = true;
			}

			Warning(LOCATION, "Found %f ",item.data);
		}
	}

	fclose(file_out);
} 



