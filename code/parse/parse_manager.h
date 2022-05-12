#pragma once

#include "globalincs/systemvars.h"
#include "globalincs/pstypes.h"
#include "parse/parselo.h"
#include "parse/parse_settings.h"
#include "parse/parse_items.h"

// TODO decide how to handle mixed tables.  Possible or not?
// TODO decide on whether we need separate table managers


template <typename T> 
struct table_data {
 
	size_t entry_count; 		// how many of this type of entry have we seen, for comparison
	SCP_string file_path;		// which file did it come from?
	size_t line;
	flagset<PI_Settings::Data_Flags> flags;				// what line did it come from
	T data;						


	table_data(size_t count_in, SCP_string filep_in, size_t line_in, T data_in){
		entry_count = count_in;
		file_path = filep_in;
		line = line_in;
		data = data_in;
	}
};

struct table_entry_option_address {
	PI_Settings::Entry_Types type;
	SCP_string entry_name;
	SCP_string parse_option; // which parse item, but on the parse items which can take data
};

// required to use the table_entry_option_address as the key to the maps.
bool operator==(const table_entry_option_address& left, const table_entry_option_address& right) {
   	return left.type == right.type && left.entry_name == right.entry_name && left.parse_option == right.parse_option;
}

// just a basic hash function to allow us to use table_entry_option_address as a key for unordered map
struct entry_addresses_hash // <table_entry_option_address>
{
	std::size_t operator()(const table_entry_option_address& k) const noexcept
	{
		// Compute individual hash values and combine them using XOR and bit shifting:
		return ((std::hash<int>{}(static_cast<int>(k.type)))
				^ (std::hash<std::string>{}(k.entry_name) << 1)
				^ (std::hash<std::string>{}(k.parse_option) >> 1));
	}
};


struct table_entry_options{
	PI_Settings::Entry_Types type;
	SCP_string entry_name;
	flagset<PI_Settings::Entry_Flags> flags;
	SCP_string inheritance_target; // templates and variants
	SCP_unordered_map<size_t, size_t> anonymous_count; // if a table has an anonymous subsection, we need to count them to differentiate
};

// These hold the data for the table entries before being placed into the main struct
struct aggregate_table_data {
	// Each map holds a different type of incoming information.
	// Tell me exactly which option for which entry you need, 
	// and I'll tell you what it is, where it came from and 
	// whether it is before or after other data that has come in."	
	// USAGE_NOTE and MARKER entry types don't need data management.
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<SCP_string>>, entry_addresses_hash>	strings;	//	These are combined: STRING, XSTR, Filenames
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<int>>, entry_addresses_hash>		ints;
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<int64_t>>, entry_addresses_hash>	longs;
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<float>>, entry_addresses_hash>		floats;
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<double>>, entry_addresses_hash>		doubles;	
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<vec3d>>, entry_addresses_hash>		vec3s;
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<bool>>, entry_addresses_hash>		bools;	
	// The only difference with these is that table data holds a vector.
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<SCP_vector<SCP_string>>>, entry_addresses_hash>	string_lists;	
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<SCP_vector<int>>>, entry_addresses_hash>		int_lists;
	SCP_unordered_map<table_entry_option_address, SCP_vector<table_data<SCP_vector<float>>>, entry_addresses_hash>		float_lists;
			
};


class table_manager {

	friend class parse_item_builder;

private:

	PI_Settings::Table_Types _table_type;  // The table type we are parsing
	flagset<PI_Settings::Table_Flags> _table_flags; // TODO make sure this is initially false

	SCP_vector<parse_item> _all_items; 	//  holds all the information pertaining to how to parse table options
	std::unordered_map<SCP_string, uint> _item_indices;  	// for quick look up in _all_items for whatever parse item we need.
	bool _indexed; // have we populated _item_indices yet

	aggregate_table_data 	_accepted_data; //  all the data that has been received from the tables
	table_entry_option_address _current_entry_address; // the current key access the data in _accepted_data
	SCP_vector<SCP_string> _entry_heirarchy; // The name of the current entry, including sub-entry names
	SCP_unordered_map<PI_Settings::Entry_Types, size_t> _current_anon_entry_indices; // used to help generate names for anonymous subsections

	SCP_unordered_map<PI_Settings::Entry_Types, SCP_vector<table_entry_options>> _table_entries; 	// Holds the table_entries that have been found so far. no indexing. 
	std::pair<PI_Settings::Entry_Types, size_t> _current_entry; 	// the current entry from _table_entries, TODO set 
	SCP_unordered_map<PI_Settings::Entry_Types, size_t> _entry_type_counts;	// counting how many of each type has been encountered, even repeats of already encountered entries are counted

	// these two items keep track of where each item of data is coming from, to enable table blame.
	SCP_string _current_filepath; 
	int _current_line;




	bool _activate_item(SCP_string key); // used for markers or another type that does not require data reading
	void _add_data(SCP_string key);	// add a value from a table to the table of values
	char _get_delimiter();	// quickly return this table's delimiter, usually ":"
	void index_search_strings();	// add _all_items names to _item_indices so that we can look up indices quickly


public:


	void parse_table_file(); // Add table data to the table entry data holder class.
	void validate_parse_items(); // just check that nothing is obviously wrong with the incoming parse items.
	void dump_to_file();
	void test_dump(SCP_string filename);
	PI_Settings::Table_Types get_table_type(){ return _table_type; } 

	size_t get_size() {return _all_items.size(); }

};

SCP_string get_table_filename_from_type(PI_Settings::Table_Types type_in);
SCP_string get_parse_item_type_description_from_type(PI_Settings::Item_Types type_in);

void init_parse_tables();

