#pragma once

#include "globalincs/pstypes.h"
#include "weapon/weapon.h"

// necessary defines

#define MAX_WING_SLOTS	4
#define MAX_WING_BLOCKS	3
#define	MAX_WSS_SLOTS	(MAX_WING_BLOCKS*MAX_WING_SLOTS)

#define WING_SLOT_FILLED				(1<<0)
#define WING_SLOT_EMPTY					(1<<1)
#define WING_SLOT_IS_PLAYER			(1<<3)
#define WING_SLOT_LOCKED				(1<<4)
#define WING_SLOT_SHIPS_DISABLED		(1<<5)
#define WING_SLOT_WEAPONS_DISABLED		(1<<6)

#define WING_SLOT_DISABLED			(WING_SLOT_SHIPS_DISABLED|WING_SLOT_WEAPONS_DISABLED)
#define WING_SLOT_IGNORE_SHIPS		(WING_SLOT_SHIPS_DISABLED|WING_SLOT_LOCKED)
#define WING_SLOT_IGNORE_WEAPONS	(WING_SLOT_WEAPONS_DISABLED|WING_SLOT_LOCKED)

// different operations used in xx_apply()
#define WSS_DUMP_TO_LIST		0
#define WSS_GRAB_FROM_LIST		1
#define WSS_SWAP_SLOT_SLOT		2
#define WSS_SWAP_LIST_SLOT		3

// icons
#define NUM_ICON_FRAMES					6
#define ICON_FRAME_NORMAL				0
#define ICON_FRAME_HOT					1
#define ICON_FRAME_SELECTED			2
#define ICON_FRAME_PLAYER				3
#define ICON_FRAME_DISABLED			4
#define ICON_FRAME_DISABLED_HIGH		5


//////////////////////////////////////////////
// Slots
//////////////////////////////////////////////

// What??? You're telling me we can have all the info for a loadout slot in one place????
// Yes, Volition, we can
typedef struct loadout_slot {
	// from the old ss_slot_info struct
	int status;   // slot status (WING_SLOT_DISABLED, etc)
	int sa_index; // index into ship arrival list, -1 if ship is created
	int original_ship_class;
	bool in_mission;

	// from the old ws_unit class
	int	ship_class;
	int wing_index;
	int	primaries[MAX_SHIP_PRIMARY_BANKS];
	int secondaries[MAX_SHIP_SECONDARY_BANKS];
	int	wep_count[MAX_SHIP_SECONDARY_BANKS];

	// from the old ss_wing_info struct -- just set it on each ship in a wing and call it a day.
	// no need for separate abstraction.
	bool is_late;

} loadout_slot;


//////////////////////////////////////////////
// Saving loadout, *after* selections have been made
//////////////////////////////////////////////
typedef struct loadout_data 
{
	char				filename[MAX_FILENAME_LEN];				// mission filename
	char				last_modified[DATE_TIME_LENGTH];	// when mission was last modified
	SCP_vector<loadout_slot>			unit_data;			// ship and weapon data
	int				weapon_pool[MAX_WEAPON_TYPES];	// available weapons
	int				ship_pool[MAX_SHIP_CLASSES];			// available ships
} loadout_data;

extern loadout_data Player_loadout;

// Manages how many ships and weapons are available and are currently in each ship
class loadout_manager {
	int _current_team;	// for calls that assume that the team is set.

	SCP_vector<SCP_vector<loadout_slot>> _slots;
	SCP_vector<loadout_slot> _loaded_slots; // slots from loading saved loadouts.

	// ship info and weapon info indexes are keys, value is the count. 
	// Ordered maps so that we can interate when doing UI stuff.
	SCP_vector<SCP_map<int, int>> _ship_pool;
	SCP_vector<SCP_map<int, int>> _weapon_pool;

public:

	/////// Functions to manage slot contents
	// NOTE: To reduce the effort of the refactor, functions work with the current team unless otherwise specified.

	void set_team(int team);

	// get info stored, assumes _current_team is set correctly.
	int get_team();
	int get_number_of_slots();

	int get_ship_status(int slot_index);
	int get_sa_index(int slot_index); // index into ship arrival list, -1 if ship is created
	int get_original_ship_class(int slot_index);
	bool is_in_mission(int slot_index);
	bool is_late(int slot_index);

	int get_ship_class(int slot_index);
	int get_weapon(int slot_index, int bank_index, bool primary);
	int get_weapon_count(int slot_index, int bank_index);
	int get_wing_index(int slot_index);
	int get_wing_count();


	int team_get_ship_class(int team, int slot_index);
	int team_get_weapon(int team, int slot_index, int bank_index, bool primary);
	int team_get_weapon_count(int team, int slot_index, int bank_index);
	int team_get_wing_index(int team, int slot_index);
	int team_get_wing_count(int team);

	int team_get_ship_status(int team, int slot_index);
	int team_get_sa_index(int team, int slot_index);
	int team_get_original_ship_class(int team, int slot_index);
	bool team_is_in_mission(int team, int slot_index);
	bool team_is_late(int team, int slot_index);

	void set_ship_status(int slot_index, int flags);
	void set_sa_index(int slot_index, int index); 
	void set_original_ship_class(int slot_index, int ship_class);
	void set_is_in_mission(int slot_index, bool in_mission);
	void set_is_late(int slot_index, bool late);

	void set_ship_class(int slot_index, int ship_class);
	void set_weapon(int slot_index, int bank_index, int weapon, bool primary);
	void set_weapon_count(int slot_index, int bank_index, int weapon_count);
	void set_wing_index(int slot_index, int wing_index);

	void team_set_ship_class(int team, int slot_index, int ship_class);
	void team_set_weapon(int team, int slot_index, int bank_index, int weapon, bool primary);
	void team_set_weapon_count(int team, int slot_index, int bank_index, int weapon_count);
	void team_set_wing_index(int team, int slot_index, int wing_index);

	void team_set_ship_status(int team, int slot_index, int flags);
	void team_set_sa_index(int team, int slot_index, int index); 
	void team_set_original_ship_class(int team, int slot_index, int ship_class);
	void team_set_is_in_mission(int team, int slot_index, bool in_mission);
	void team_set_is_late(int team, int slot_index, bool late);

	// status functions
	bool is_ship_slot_weaponless(int slot_index);
	bool is_bank_filled(int slot_index, int bank, bool primary);
	bool are_all_slots_shipless();

	void swap_weapon_slots(int slot_index, int bank_a, int bank_b, bool primary);

	// for scripting access
	loadout_slot* get_slot(int slot_index);

	// TODO Finish me!
	void empty_slot_into_pool(int slot_index);
	void empty_bank_and_refill_pool(int slot_index, int bank, bool primary);
	void empty_primary_bank_and_refill_pool(int slot_index, int bank);
	void empty_secondary_bank_and_refill_pool(int slot_index, int bank);

	// TODO Finish me!
	void reset_ship_pool(team_data *pteam);
	void team_reset_all_slots(int team);

	SCP_vector<int> get_active_ships();

	// This is a very important function for making things dynamic.
	int get_slot_via_wing_and_ship(int wing, int ship);

	// To assist with CSG and loadout saves.
	void add_loaded_slot(const loadout_slot& slot);
	void clear_loaded_slots();

	// TODO finish me!
	void apply_default_weapons(int slot_index);
	void maybe_restore_loadout();
};

extern loadout_manager Loadouts;
