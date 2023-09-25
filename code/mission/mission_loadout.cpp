#include "mission/mission_loadout.h"
#include "globalincs/globals.h"

// TODO! .... all these assertions need messages.  Why did I wait???

// what the ship and weapon loadout is... used since we want to use the 
// same loadout if the mission is played again
loadout_data Player_loadout;	

// What the player is currently choosing for its squadron's loadouts
loadout_manager Loadouts;

int loadout_manager::get_team() { return _current_team; }

int loadout_manager::get_number_of_slots() { return static_cast<int>(_slots[_current_team].size()); }

int loadout_manager::get_ship_status(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].status;
}

int loadout_manager::get_sa_index(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].sa_index;
}

int loadout_manager::get_original_ship_class(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].original_ship_class;
}

bool loadout_manager::is_in_mission(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].in_mission;
}

bool loadout_manager::is_late(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].is_late;
}


int loadout_manager::get_ship_class(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].ship_class;
}

// TODO: Fix calls to use primary bool.
int loadout_manager::get_weapon(int slot_index, int bank_index, bool primary) 
{ 
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size())); 
	
	if (primary){
		Assert(bank_index > -1 && bank_index < MAX_SHIP_PRIMARY_BANKS);
		return _slots[_current_team][slot_index].primaries[bank_index];
	} else{
		Assert(bank_index > -1 && bank_index < MAX_SHIP_SECONDARY_BANKS);
		return _slots[_current_team][slot_index].secondaries[bank_index];
	}
}

// TODO: Fix calls to make bank_index obey secondary index instead overall bank index.
int loadout_manager::get_weapon_count(int slot_index, int bank_index) 
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size())); 
	Assert(bank_index > -1 && bank_index < MAX_SHIP_SECONDARY_BANKS);

	return _slots[_current_team][slot_index].wep_count[bank_index];
}

int loadout_manager::get_wing_index(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	return _slots[_current_team][slot_index].wing_index;
}

int loadout_manager::team_get_ship_class(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_ship_class(slot_index);

	set_team(old_team);
	
	return value;
}

int loadout_manager::team_get_weapon(int team, int slot_index, int bank_index, bool primary)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_weapon(slot_index, bank_index, primary);

	set_team(old_team);
	
	return value;
}
	
int loadout_manager::team_get_weapon_count(int team, int slot_index, int bank_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_weapon_count(slot_index, bank_index);

	set_team(old_team);
	return value;
}

int loadout_manager::team_get_wing_index(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_wing_index(slot_index);

	set_team(old_team);
	return value;
}

extern int ship_info_size();

void loadout_manager::set_ship_class(int slot_index, int ship_class)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(ship_class > -2 && ship_class < ship_info_size()); // negative 2 intentional, since -1 means empty.

	_slots[_current_team][slot_index].ship_class = ship_class;
}

void loadout_manager::set_weapon(int slot_index, int bank_index, int weapon, bool primary)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(bank_index > -1 && bank_index < ((primary) ? MAX_SHIP_PRIMARY_BANKS : MAX_SHIP_SECONDARY_BANKS));
	Assert(weapon > -2 && weapon < weapon_info_size()); // negative 2 intentional, since -1 means empty.

	if (primary)
		_slots[_current_team][slot_index].primaries[bank_index] = weapon;
	else
		_slots[_current_team][slot_index].secondaries[bank_index] = weapon;
}

void loadout_manager::set_weapon_count(int slot_index, int bank_index, int weapon_count)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(bank_index > -1 && bank_index < MAX_SHIP_WEAPONS);

	_slots[_current_team][slot_index].wep_count[bank_index] = weapon_count;
}

void loadout_manager::set_wing_index(int slot_index, int wing_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(wing_index > -2); // negative 1 is just a ship without a wing.

	_slots[_current_team][slot_index].wing_index = wing_index;
}

void loadout_manager::set_team(int team)
{
	Assert(team > -1 && team < static_cast<int>(_slots.size()));

	if (team > -1 && team < static_cast<int>(_slots.size())){
		_current_team = team;
	} else {
		_current_team = 0;
	}
}

void loadout_manager::team_set_ship_class(int team, int slot_index, int ship_class)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_ship_class(slot_index, ship_class);
	set_team(old_team);
}

// this cannot accept legacy method of combined primary and secondary bank indices.
void loadout_manager::team_set_weapon(int team, int slot_index, int bank_index, int weapon, bool primary)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_weapon(slot_index, bank_index, weapon, primary);
	set_team(old_team);
}

void loadout_manager::team_set_weapon_count(int team, int slot_index, int bank_index, int weapon_count)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_weapon_count(slot_index, bank_index, weapon_count);
	set_team(old_team);
}

void loadout_manager::team_set_wing_index(int team, int slot_index, int wing_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_wing_index(slot_index, wing_index);
	set_team(old_team);
}

void loadout_manager::team_set_ship_status(int team, int slot_index, int flags)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_ship_status(slot_index, flags);
	set_team(old_team);
}

void loadout_manager::team_set_sa_index(int team, int slot_index, int index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_sa_index(slot_index, index);
	set_team(old_team);
}

void loadout_manager::team_set_original_ship_class(int team, int slot_index, int ship_class)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_original_ship_class(slot_index, ship_class);
	set_team(old_team);
}

void loadout_manager::team_set_is_in_mission(int team, int slot_index, bool in_mission)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_is_in_mission(slot_index, in_mission);
	set_team(old_team);
}

void loadout_manager::team_set_is_late(int team, int slot_index, bool late)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.

	int old_team = _current_team;

	set_team(team);
	set_is_in_mission(slot_index, late);
	set_team(old_team);
}


// TODO: Fix calls to use the bool primary and secondary.
bool loadout_manager::is_bank_filled(int slot_index, int bank_index, bool primary)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size())); 
	
	if (primary) {
		Assert(bank_index > -1 && bank_index < MAX_SHIP_PRIMARY_BANKS);
		return _slots[_current_team][slot_index].primaries[bank_index] > -1;
	} else {
		Assert(bank_index > -1 && bank_index < MAX_SHIP_SECONDARY_BANKS);
		return (_slots[_current_team][slot_index].secondaries[bank_index] > -1) && (_slots[_current_team][slot_index].wep_count[bank_index] > 0);
	} 
}

bool loadout_manager::is_ship_slot_weaponless(int index)
{
	auto& slot = _slots[_current_team][index];
	
	for (int i = 0; i < MAX_SHIP_PRIMARY_BANKS; ++i) {
		if (slot.primaries[i] > -1){
			return false;
		}
	}

	for (int i = 0; i < MAX_SHIP_PRIMARY_BANKS; ++i) {
		if (slot.secondaries[i] > -1 && slot.wep_count[i] > 0){
			return false;
		}
	}

	return true;
}

bool loadout_manager::are_all_slots_shipless()
{
	if (_slots[_current_team].empty()) {
		return true;
	}

	for ( auto& slot : _slots[_current_team] ) {
		if ( slot.ship_class >= 0 ) 
			return false;
	}

	return true;
}

// TODO: FINISH ME!
void loadout_manager::apply_default_weapons(int slot_index)
{
//	Assert(_current_team > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

//	int ship_class = _slots[_current_team][slot_index].ship_class;

	
}

// TODO! Safety Checks
// for retrieving a pointer to a slot for scripting access.
loadout_slot* loadout_manager::get_slot(int index)
{
	return &_slots[_current_team][index];
}

void loadout_manager::swap_weapon_slots(int slot_index, int bank_a, int bank_b, bool primary)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(bank_a > -1 && bank_a < ((primary) ? MAX_SHIP_PRIMARY_BANKS : MAX_SHIP_SECONDARY_BANKS));
	Assert(bank_b > -1 && bank_b < ((primary) ? MAX_SHIP_PRIMARY_BANKS : MAX_SHIP_SECONDARY_BANKS));

	auto& slot = _slots[_current_team][slot_index];

	if (primary) {
		int temp = slot.primaries[bank_a];
		slot.primaries[bank_a] = slot.primaries[bank_b];
		slot.primaries[bank_b] = temp;
	} else {
		int temp = slot.secondaries[bank_a];
		slot.secondaries[bank_a] = slot.secondaries[bank_b];
		slot.secondaries[bank_b] = temp;

		temp = slot.wep_count[bank_a];
		slot.wep_count[bank_a] = slot.wep_count[bank_b];
		slot.wep_count[bank_b] = temp;
	}
}

// Instead of recording how many wings we're using, we can use the relatively slower
// method of looping through slots because this function should only be used by UI.
// Also, we will closer to making the number of wings dynamic if we do it this way.
int loadout_manager::get_wing_count()
{
	SCP_unordered_set<int> found;
	int count = 0;

	for (auto& slot : _slots[_current_team]){
		if (slot.wing_index > -1 && found.insert(slot.wing_index).second){
			++count;
		}
	}	

	return found.size();
}

int loadout_manager::team_get_wing_count(int team)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_wing_count();

	set_team(old_team);

	return value;
}

int loadout_manager::team_get_ship_status(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_ship_status(slot_index);

	set_team(old_team);

	return value;
}

int loadout_manager::team_get_sa_index(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_sa_index(slot_index);

	set_team(old_team);

	return value;
}

int loadout_manager::team_get_original_ship_class(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = get_original_ship_class(slot_index);

	set_team(old_team);

	return value;
}

bool loadout_manager::team_is_in_mission(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = is_in_mission(slot_index);

	set_team(old_team);

	return value;
}

bool loadout_manager::team_is_late(int team, int slot_index)
{
	Assert(team > - 1 && team < static_cast<int>(_slots.size()));

	// best way to direct access team-specific data is to temporarily set the team to 
	// the desired value until we've retrived that data. This is to reduce duplicate code.
	int old_team = _current_team;

	set_team(team);	

	int value = is_late(slot_index);

	set_team(old_team);

	return value;
}

void loadout_manager::set_ship_status(int slot_index, int flags)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	_slots[_current_team][slot_index].status = flags;
}

void loadout_manager::set_sa_index(int slot_index, int index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	_slots[_current_team][slot_index].sa_index = index;
}

void loadout_manager::set_original_ship_class(int slot_index, int ship_class)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(ship_class > -2 && ship_class < ship_info_size());

	_slots[_current_team][slot_index].original_ship_class = ship_class;
}

void loadout_manager::set_is_in_mission(int slot_index, bool in_mission)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	_slots[_current_team][slot_index].in_mission = in_mission;
}

void loadout_manager::set_is_late(int slot_index, bool late)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	_slots[_current_team][slot_index].is_late = late;
}


void loadout_manager::reset_ship_pool(team_data *pteam)
{
	_ship_pool.clear();

	// set number of available ships based on counts in team_data
	for (auto& ship : pteam->ship_list) {
		_ship_pool[_current_team].insert({ship.first, ship.second});
	}
}

SCP_vector<int> loadout_manager::get_active_ships()
{
	SCP_vector<int> values;

	for (auto ship : _ship_pool[_current_team]) {
		if (ship.second > 0){
			values.push_back(ship.first);
		}
	}

	return values;
}

int loadout_manager::get_slot_via_wing_and_ship(int wing, int ship)
{
	Assert(wing > -1);
	Assert(ship > -1);

	SCP_unordered_set<int> found;
	int wing_count = -1;
	int slot_index = -1;

	for (auto& slot : _slots[_current_team]){
		++slot_index;
		
		if (slot.wing_index > -1 && found.insert(slot.wing_index).second){
			++wing_count;

			if (wing == wing_count){
				break;
			}
		}
	}

	// didn't find the ship....
	if ((slot_index == -1) || (wing_count == -1) || (wing != _slots[_current_team][slot_index + ship].wing_index))
		return -1;

	return slot_index + ship;
}

void loadout_manager::empty_primary_bank_and_refill_pool(int slot_index, int bank_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(bank_index > -1 && bank_index < MAX_SHIP_PRIMARY_BANKS);

	auto& slot = _slots[_current_team][slot_index];
	auto& pool = _weapon_pool[_current_team];

	if (slot.primaries[bank_index] > -1){
		if (pool.find(slot.primaries[bank_index]) != pool.end()){
			++pool[slot.primaries[bank_index]];
		} else {
			pool[slot.primaries[bank_index]] = 1;
		}

		slot.primaries[bank_index] = -1;
	}
}

void loadout_manager::empty_secondary_bank_and_refill_pool(int slot_index, int bank_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));
	Assert(bank_index > -1 && bank_index < MAX_SHIP_SECONDARY_BANKS);

	auto& slot = _slots[_current_team][slot_index];
	auto& pool = _weapon_pool[_current_team];

	if (slot.secondaries[bank_index] > -1){
		if (pool.find(slot.primaries[bank_index]) != pool.end()){
			pool[slot.secondaries[bank_index]] += slot.wep_count[bank_index];
		} else {
			pool[slot.secondaries[bank_index]] = slot.wep_count[bank_index];
		}
		
		slot.secondaries[bank_index] = -1;
		slot.wep_count[bank_index] = 0;
	}
}

void loadout_manager::empty_bank_and_refill_pool(int slot_index, int bank_index, bool primary)
{
	if (primary)
		empty_primary_bank_and_refill_pool(slot_index, bank_index);
	else 
		empty_secondary_bank_and_refill_pool(slot_index, bank_index);
}

void loadout_manager::empty_slot_into_pool(int slot_index)
{
	Assert(slot_index > -1 && slot_index < static_cast<int>(_slots[_current_team].size()));

	for (int i = 0; i < MAX_SHIP_PRIMARY_BANKS; ++i) {
		empty_primary_bank_and_refill_pool(slot_index, i);
	}
	
	for (int i = 0; i < MAX_SHIP_SECONDARY_BANKS; ++i) {
		empty_secondary_bank_and_refill_pool(slot_index, i);
	}

	auto& slot = _slots[_current_team][slot_index];
	auto& pool = _ship_pool[_current_team];

	if (slot.ship_class > -1){
		if (pool.find(slot.ship_class) != pool.end()){
			++pool[slot.ship_class];
		} else {
			pool[slot.ship_class] = 1;
		}

		slot.ship_class = -1;
	}
}

void loadout_manager::add_loaded_slot(const loadout_slot& slot)
{
	_loaded_slots.push_back(slot);
}

void loadout_manager::clear_loaded_slots() { _loaded_slots.clear(); };

// restore ship/weapons loadout from the Player_loadout struct
void loadout_manager::maybe_restore_loadout()
{
	// only restore if mission hasn't changed
	if ( stricmp(Player_loadout.last_modified, The_mission.modified) != 0 ) {
		return;
	}

	SCP_unordered_map<int, int> ships_available; std::copy(_ship_pool[_current_team].begin(), _ship_pool[_current_team].end(), ships_available);
	SCP_unordered_map<int, int> weapons_available; std::copy(_weapon_pool[_current_team].begin(), _weapon_pool[_current_team].end(), weapons_available);
	SCP_unordered_map<int, int> ships_required_by_load;
	SCP_unordered_map<int, int> weapons_required_by_load;

	// sorry for using gross c++ tuples.  It just seems like a waste to make a new struct just to use in this function.
	SCP_vector<std::tuple<int, int, int, int>> extra_missiles_requested;

	// add the ships and weapons used in the slots already.
	for (auto& slot : _slots[_current_team]) {
		// check that we have a valid slot
		if (!(slot.status & WING_SLOT_LOCKED)){
			// if the slot is diabled, it should not be adjusted
			if (!(slot.status & WING_SLOT_SHIPS_DISABLED)){
				// check that we have a ship.
				if (slot.ship_class > -1){
					// add it to the avialable list.
					if (ships_available.find(slot.ship_class) != ships_available.end()){
						++ships_available[slot.ship_class];
					} else {
						ships_available[slot.ship_class] = 1;
					}
				}
			}

			// same process for weapons
			if (!(slot.status & WING_SLOT_WEAPONS_DISABLED)){
				for (auto& primary : slot.primaries){
					if (primary > -1){
						if (weapons_available.find(primary) != weapons_available.end()){
							++weapons_available[primary];
						} else {
							weapons_available[primary] = 1;
						}
					}
				}
  
				for (int i = 0; i < MAX_SHIP_SECONDARY_BANKS; ++i){
					if (slot.secondaries[i] > -1){
						if (weapons_available.find(slot.secondaries[i]) != weapons_available.end()){
							weapons_available[slot.secondaries[i]] += slot.wep_count[i];
						} else {
							weapons_available[slot.secondaries[i]] = slot.wep_count[i];
						}
					}
				}
			}
		}
	}

	// TODO! We have to make sure that each ship matches up, probably by ship name. Should definitely check
	// when applying changes.  Need to save slot status to save game. :(
	// this code system is baaaaaaaaaad 


	// Count what the Player_loadout record contains
	for (auto& recorded_slot : Player_loadout.unit_data) {

		if (!(recorded_slot.status & WING_SLOT_LOCKED)){

			// TODO! We need to verify that a specific ship can be used.
			if (!(recorded_slot.status & WING_SLOT_SHIPS_DISABLED)){
				if ((recorded_slot.ship_class > -1) && (recorded_slot.ship_class < ship_info_size())) {
					if (ships_required_by_load.find(recorded_slot.ship_class) != ships_required_by_load.end()){
						++ships_required_by_load[recorded_slot.ship_class];
					} else {
						ships_required_by_load[recorded_slot.ship_class] = 1;
					}
				}
			}

			// TODO! we need to verify that each bank can be used by the desired choice, and that each bank 
			// is on the current mod's version of the fighter before adding their counts.

			if (!(recorded_slot.status & WING_SLOT_WEAPONS_DISABLED)){
				for (auto& primary : recorded_slot.primaries){
					if (primary > -1){
						if (weapons_required_by_load.find(primary) != weapons_required_by_load.end()){
							++weapons_required_by_load[primary];
						} else {
							weapons_required_by_load[primary] = 1;
						}
					}
				}

				for (int i = 0; i < MAX_SHIP_SECONDARY_BANKS; ++i){
					// TODO, add validity check that allows us to account for changed ammo sizes.
					// Use the utility function to let us look up the new capacity.... 
					// What should I do if the current count is below the new maximum?
					if (recorded_slot.secondaries[i] > -1){
						if (weapons_required_by_load.find(recorded_slot.secondaries[i]) != weapons_required_by_load.end()){
							weapons_required_by_load[i] += recorded_slot.wep_count[i]; 
						} else {
							weapons_required_by_load[i] = recorded_slot.wep_count[i];
						}
					}
				}
			}
		}
	}

	// if any ship or any weapon does not have enough to cover the saved loadout's requirements, just abort.
	// We simply can't read the mind of the user.
	for (auto& ship : ships_required_by_load){
		if (ships_available.find(ship.first) == ships_available.end() || ships_available[ship.first] < ship.second){
			return;
		}
	}

	for (auto& weapon : weapons_required_by_load){
		if (weapons_available.find(weapon.first) == ships_available.end() || weapons_available[weapon.first] < weapon.second){
			return;
		}
	}

	// once I have ship names set up, I'm going to verify them before applying, but for now, just assume everything is in the right order....

	// set the weapons and reduce the weapons available.
	for (int i = 0; i < static_cast<int>(_slots[_current_team].size()) && i < static_cast<int>(Player_loadout.unit_data.size()); ++i){
		auto& from_slot = _slots[_current_team][i];
		auto& to_slot = Player_loadout.unit_data[i];

		if (to_slot.status & WING_SLOT_LOCKED)
			continue;

		if (to_slot.status & WING_SLOT_SHIPS_DISABLED){
			set_ship_class(i, from_slot.ship_class);
			--ships_available[from_slot.ship_class];
			Assert(ships_available[from_slot.ship_class] > -1);
		}

		if (to_slot.status & WING_SLOT_WEAPONS_DISABLED){
			for (int j = 0; j < MAX_SHIP_PRIMARY_BANKS; ++j){
				set_weapon(i, j, from_slot.primaries[j], true);
				--weapons_available[from_slot.primaries[j]];
				Assert(weapons_available[from_slot.primaries[j]] > -1);
			}

			for (int j = 0; j < MAX_SHIP_SECONDARY_BANKS; ++j){
				set_weapon(i, j, from_slot.secondaries[j], false);
				set_weapon_count(i, j, from_slot.wep_count[j]);
				weapons_available[from_slot.secondaries[j]] -= from_slot.wep_count[j];
				Assert(weapons_available[from_slot.primaries[j]] > -1);
			}
		}
	}
}
