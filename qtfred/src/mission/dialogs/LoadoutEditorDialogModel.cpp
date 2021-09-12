#include "LoadoutEditorDialogModel.h"
#include "FredApplication.h"
#include "mission/missionparse.h"
#include "object/object.h"
#include "mission/Editor.h"
#include "ship/ship.cpp"
#include "weapon/weapon.h"
#include <QMessageBox>

namespace fso {
namespace fred {
namespace dialogs {

// for variable-enabled weapons and ships, these are the defaults.
constexpr int ShipVarDefault = 4;
constexpr int WeaponVarDefault = 40;

LoadoutDialogModel::LoadoutDialogModel(QObject* parent, EditorViewport* viewport) 
		: AbstractDialogModel(parent, viewport)
{
		initializeData();
}

void LoadoutDialogModel::initializeData()
{
	_currentTeam = 0;

	TeamLoadout defaultEntry;
	// make sure we have the correct number of teams.
	for (int i = 0; i < Num_teams; i++) {
		_teams.emplace_back(defaultEntry);
	}

	// this is basically raw data, so we have to make sure to calculate the indices correctly.
	SCP_vector<int> usage = _editor->getStartingWingLoadoutUseCounts();

	Assert(usage.size() == MAX_SHIP_CLASSES * MAX_TVT_TEAMS * 2);

	int currentTeam = 0;
	int shipIndex = 0;
	int index = 0;

	for (auto& ship : Ship_info){
		if (ship.flags[Ship::Info_Flags::Player_ship]) {
			for (auto& team : _teams) {

				// even though it gets created here, it's not complete because the enabled entries are in Team_data
				team.ships.emplace_back(
					index,
					false,
					usage.at((MAX_SHIP_CLASSES * currentTeam) + index),
					0,
					-1,
					QString(ship.name).toStdString());// definitely looks janky, but Qstring handles the string conversion better.

				// make sure that starting ships are enabled.
				if (team.ships.back().countInWings > 0) {
					team.ships.back().enabled = true;
				}

				// while it's easier to figure out the number used in wings count, add it to the total
				_teams[currentTeam].startingShipCount += team.ships.back().countInWings;
				

				currentTeam++;

			}
			currentTeam = 0;
		}

		index++;
	}

	index = 0;

	for (auto& weapon : Weapon_info){
		if (weapon.wi_flags[Weapon::Info_Flags::Player_allowed]) {
			for (auto& team : _teams) {
				// even though it gets created here, it's not complete because the enabled entries are in Team_data
				team.weapons.emplace_back(
					index,
					false,
					usage[(MAX_SHIP_CLASSES * MAX_TVT_TEAMS) + (MAX_SHIP_CLASSES * currentTeam) + index],
					0,
					-1,
					QString(weapon.name).toStdString());// definitely looks janky, but Qstring handles the string conversion better.

				// make sure that weapons in starting wing slots are enabled.
				if (team.weapons.back().countInWings > 0) {
					team.weapons.back().enabled = true;
				}

				currentTeam++;
			}
			currentTeam = 0;
		}

		index++;
	}

	currentTeam = 0;

	for (auto& team : Team_data) {		

		for (index = 0; index < team.num_ship_choices; index++){
			// if it has an enabling variable, add it to the correct vector.
			if (strlen(team.ship_list_variables[index])) {
				_teams[currentTeam].varShips.emplace_back(
					get_index_sexp_variable_name(team.ship_list_variables[index]),
					true,
					0, // 0 until proven otherwise in-game.
					team.ship_count[index],
					(strlen(team.ship_count_variables[index])) ? get_index_sexp_variable_name(team.ship_list_variables[index]) : -1,
					SCP_string(team.ship_list_variables[index])
				);

			} // if it doesn't, enable the matching item.
			else {
				for (auto item : _teams[currentTeam].ships) {
					if (team.ship_list[index] == item.infoIndex) {
						item.enabled = true;
						item.extraAllocated = team.ship_count[index];

						if (strlen(team.ship_count_variables[index])) {
							item.varCountIndex = get_index_sexp_variable_name(team.ship_count_variables[index]);
						}
						else {
							item.varCountIndex = -1;
						}

						if (Ship_info[item.infoIndex].num_primary_banks > _teams[currentTeam].largestPrimaryBankCount) {
							_teams[currentTeam].largestPrimaryBankCount = Ship_info[item.infoIndex].num_primary_banks;
						}

						int capacity = 0;

						for (int i = 0; i < Ship_info[item.infoIndex].num_secondary_banks; i++){
							capacity += Ship_info[item.infoIndex].secondary_bank_ammo_capacity[i];
						}
							
						if (capacity > _teams[currentTeam].largestSecondaryCapacity) {
							_teams[currentTeam].largestSecondaryCapacity = capacity;
						}

						break;
					}
				}
			}
		}

		for (index = 0; index < team.num_weapon_choices; index++){
			// if it has an enabling variable, add it to the correct vector.
			if (strlen(team.weaponry_pool_variable[index])) {
				_teams[currentTeam].varWeapons.emplace_back(
					get_index_sexp_variable_name(team.weaponry_pool_variable[index]),
					true,
					0, // 0 until proven otherwise in-game.
					team.weaponry_count[index],
					(strlen(team.weaponry_amount_variable[index])) ? get_index_sexp_variable_name(team.weaponry_pool_variable[index]) : -1,
					SCP_string(team.weaponry_pool_variable[index])
				);

				// it's impossible for this type to tell if it's secondary or its cargo size, so this default allows for a good number.
				if (_teams[currentTeam].varWeapons.back().extraAllocated == 0) {
					_teams[currentTeam].varWeapons.back().extraAllocated = WeaponVarDefault * _teams[currentTeam].startingShipCount;
				}

			} // if it doesn't, enable the matching item.
			else {
				for (auto& item : _teams[currentTeam].weapons) {
					if (team.weaponry_pool[index] == item.infoIndex) {
						item.enabled = true;
						item.extraAllocated = team.weaponry_count[index];

						if (strlen(team.weaponry_amount_variable[index])) {
							item.varCountIndex = get_index_sexp_variable_name(team.weaponry_amount_variable[index]);
						}
						else {
							item.varCountIndex = -1;
						}

						// if none are present set the meaningful default here.
						if (item.extraAllocated == 0 && item.countInWings == 0) {
							if (Weapon_info[item.infoIndex].subtype == WP_LASER) {
								item.extraAllocated = (_teams[currentTeam].startingShipCount * _teams[currentTeam].largestPrimaryBankCount) / 2;
							}
							else {
								Assert(Weapon_info[item.infoIndex].cargo_size > 0);
								item.extraAllocated = (_teams[currentTeam].startingShipCount * _teams[currentTeam].largestSecondaryCapacity) / (2 * Weapon_info[item.infoIndex].cargo_size);
							}
						}

						break;
					}
				}
			}
		}

		currentTeam++;
		if (currentTeam >= static_cast<int>(_teams.size())) {
			break;
		}
	}

	// need to build list for count variables.
	for (auto& sexp : Sexp_variables) {
		if ((sexp.type & SEXP_VARIABLE_SET) && (sexp.type & SEXP_VARIABLE_NUMBER)){	
			_numberVarList.push_back(sexp.variable_name);
		}
	}

	buildCurrentLists();

	_currentTeam = 0;

	_playerEntryDelay = f2fl(Entry_delay_time);

	modelChanged();
}

float LoadoutDialogModel::getPlayerEntryDelay()
{
	return _playerEntryDelay;
}

void LoadoutDialogModel::setPlayerEntryDelay(float delay)
{
	_playerEntryDelay = delay;
}

int LoadoutDialogModel::getCurrentTeam()
{
	return _currentTeam + 1;
}

SCP_vector<std::pair<SCP_string, bool>> LoadoutDialogModel::getShipList()
{
	return _shipList;
}

SCP_vector<std::pair<SCP_string, bool>> LoadoutDialogModel::getWeaponList()
{
	return _weaponList;
}

SCP_vector<std::pair<SCP_string, bool>> LoadoutDialogModel::getShipEnablerVariables()
{
	return _shipVarList;
}

SCP_vector<std::pair<SCP_string, bool>> LoadoutDialogModel::getWeaponEnablerVariables()
{
	return _weaponVarList;
}

void LoadoutDialogModel::setShipInfo(SCP_string textIn, bool enabled, int extraAllocated, SCP_string varForCount)
{
	bool found = false, previouslyEnabled;
	int index = 0;

	for (auto& item : _teams[_currentTeam].ships) {
		if (textIn == Ship_info[item.infoIndex].name) {
			previouslyEnabled = !item.enabled;
			item.enabled = enabled;
			item.extraAllocated = extraAllocated;
			item.varCountIndex = get_index_sexp_variable_name(varForCount);
			SCP_string newstring = createItemString(true, index);
			found = true;

			// need to check if this ship will hold more than the last highest capacity
			if (enabled) {
				if (Ship_info[item.infoIndex].num_primary_banks > _teams[_currentTeam].largestPrimaryBankCount) {
					_teams[_currentTeam].largestPrimaryBankCount = Ship_info[item.infoIndex].num_primary_banks;
				}

				int capacity = 0;

				for (int i = 0; i < Ship_info[item.infoIndex].num_secondary_banks; i++){
					capacity += Ship_info[item.infoIndex].secondary_bank_ammo_capacity[i];
				}

				if (capacity > _teams[_currentTeam].largestSecondaryCapacity) {
					_teams[_currentTeam].largestSecondaryCapacity = capacity;
				}

				if (!previouslyEnabled && item.varCountIndex == -1) {
					item.extraAllocated = _teams[_currentTeam].startingShipCount / 2;
				}

			}

			break;
		}
	}
	
	Assert(found);

}

void LoadoutDialogModel::setWeaponInfo(SCP_string textIn, bool enabled, int extraAllocated, SCP_string varForCount)
{
	bool found = false, previouslyEnabled;
	int index = 0;

	for (auto& item : _teams[_currentTeam].weapons) {
		if (textIn == Weapon_info[item.infoIndex].name) {
			previouslyEnabled = !item.enabled;

			item.enabled = enabled;
			item.extraAllocated = extraAllocated;
			item.varCountIndex = get_index_sexp_variable_name(varForCount);

			if(item.varCountIndex == -1 && !previouslyEnabled)
			{
				if (Weapon_info[item.infoIndex].subtype == WP_LASER) {
					item.extraAllocated = (_teams[_currentTeam].startingShipCount * _teams[_currentTeam].largestPrimaryBankCount) / 2;
				}
				else {
					Assert(Weapon_info[item.infoIndex].cargo_size > 0);
					item.extraAllocated = (_teams[_currentTeam].startingShipCount * _teams[_currentTeam].largestSecondaryCapacity) / (2 * Weapon_info[item.infoIndex].cargo_size);
				}

			}

			SCP_string newstring = createItemString(false, index);
			found = true;
			break;
		}
		index++;
	}

	Assert(found);

}

SCP_string LoadoutDialogModel::createItemString(bool ship, int itemIndex)
{
	LoadoutItem* ip;
	SCP_string stringOut;

	if (ship) {
		ip = &_teams[_currentTeam].ships[itemIndex];
		stringOut = Ship_info[ip->infoIndex].name;
	} else { 
		ip = &_teams[_currentTeam].weapons[itemIndex];
		stringOut = Weapon_info[ip->infoIndex].name;
	}

	stringOut += ": ";
	stringOut += std::to_string(ip->countInWings);
	stringOut += "/";
	stringOut += std::to_string(ip->extraAllocated);
	stringOut += "/";
	stringOut += std::to_string(ip->countInWings + ip->extraAllocated);

	return stringOut;
}

void LoadoutDialogModel::copyToOtherTeam()
{
	int index = 0;

	for (auto& destinationTeam : _teams) {
		if (index == _currentTeam) {
			index++;
			continue;
		}

		destinationTeam.largestPrimaryBankCount = _teams[_currentTeam].largestPrimaryBankCount;
		destinationTeam.largestSecondaryCapacity = _teams[_currentTeam].largestSecondaryCapacity;
		destinationTeam.ships = _teams[_currentTeam].ships;
		destinationTeam.varShips = _teams[_currentTeam].varShips;
		destinationTeam.varWeapons = _teams[_currentTeam].varWeapons;
		destinationTeam.weapons = _teams[_currentTeam].weapons;

		index++;
	}
}

void LoadoutDialogModel::switchTeam(int teamIn) 
{
	if (teamIn < 1 || teamIn > static_cast<int>(_teams.size()))
		return;

	_currentTeam = teamIn - 1;

	buildCurrentLists();

	modelChanged();
}

void LoadoutDialogModel::setShipEnablerVariables(SCP_vector<SCP_string> variablesIn, bool enabled, int extraAllocated, SCP_string varForCount)
{
	for (auto& nameIn : variablesIn) {
		for (auto& variable : _shipVarList) {
			if (nameIn == variable.first) {
				variable.second = enabled;
				break;
			}
		}
	}

	// if they are enabled, we need to add them to the list.
	if (enabled) {
		for (auto& nameIn : variablesIn) {
			bool found = false;
			for (auto& item : _teams[_currentTeam].varShips) {

				// check to see if it was already added
				if (item.name == nameIn) {
					found = true;
					item.extraAllocated = extraAllocated;
					item.varCountIndex = get_index_sexp_variable_name(varForCount);
					break;
				}
			}

			if (!found) {
				_teams[_currentTeam].varShips.emplace_back(
					get_index_sexp_variable_name(nameIn),
					true,
					0, // there cannot be any because this is var enabled.
					(extraAllocated == -1) ? WeaponVarDefault * _teams[_currentTeam].startingShipCount : extraAllocated,
					get_index_sexp_variable_name(varForCount),
					SCP_string(nameIn));
			}
		}

	} // if they are disabled, remove them.
	else {
		for (auto& nameIn : variablesIn) {
			auto item = _teams[_currentTeam].varShips.begin();
			while (item != _teams[_currentTeam].varShips.end()) {
				// check to see if it was already added
				if (item->name == nameIn) {
					break;
				}
			}

			if (item != _teams[_currentTeam].varShips.end()) {
				_teams[_currentTeam].varShips.erase(item); // inefficient, but order is important here
			}
		}
	}

	modelChanged();
}

void LoadoutDialogModel::setWeaponEnablerVariables(SCP_vector<SCP_string> variablesIn, bool enabled, int extraAllocated, SCP_string varForCount)
{
	for (auto& nameIn : variablesIn) {
		for (auto& variable : _weaponVarList) {
			if (nameIn == variable.first) {
				variable.second = enabled;
				break;
			}
		}
	}

	// if they are enabled, we need to add them to the list.
	if (enabled) {
		for (auto& nameIn : variablesIn) {
			bool found = false;
			for (auto& item : _teams[_currentTeam].varWeapons) {

				// check to see if it was already added
				if (item.name == nameIn) {
					found = true;
					item.extraAllocated = extraAllocated;
					item.varCountIndex = get_index_sexp_variable_name(varForCount);
					break;
				}
			}

			if (!found) {
				_teams[_currentTeam].varShips.emplace_back(
					get_index_sexp_variable_name(nameIn),
					true,
					0, // there cannot be any because this is var enabled.
					(extraAllocated == -1) ? WeaponVarDefault * _teams[_currentTeam].startingShipCount : extraAllocated,
					get_index_sexp_variable_name(varForCount),
					SCP_string(nameIn));
			}
		}

	} // if they are disabled, remove them.
	else {
		for (auto& nameIn : variablesIn) {
			auto item = _teams[_currentTeam].varWeapons.begin();
			while (item != _teams[_currentTeam].varWeapons.end()) {
				// check to see if it was already added
				if (item->name == nameIn) {
					break;
				}
			}

			if (item != _teams[_currentTeam].varWeapons.end()) {
				_teams[_currentTeam].varWeapons.erase(item); // inefficient, but order is important here
			}
		}
	}

	modelChanged();
}


bool LoadoutDialogModel::apply() {

	int currentTeam = 0;
	int index = 0;
	int loadoutTotal = 0; // TODO Figure out if we need to set this.

	for (auto& modelTeam : _teams) {
		for (auto& ship : modelTeam.ships) {
			Team_data[currentTeam].ship_count[index] = ship.extraAllocated;

			if (ship.varCountIndex > -1) {
				strcpy_s(Team_data[currentTeam].ship_count_variables[index], Sexp_variables[ship.varCountIndex].variable_name);
			}
			else {
				memset(Team_data[currentTeam].ship_count_variables[index], 0, TOKEN_LENGTH);
			}

			Team_data[currentTeam].ship_list[index] = ship.infoIndex;
			memset(Team_data[currentTeam].ship_list_variables[index], 0, TOKEN_LENGTH);
			index++;
		}

		for (auto& ship : modelTeam.varShips) {
			Team_data[currentTeam].ship_count[index] = ship.extraAllocated;

			if (ship.varCountIndex > -1) {
				strcpy_s(Team_data[currentTeam].ship_count_variables[index], Sexp_variables[ship.varCountIndex].variable_name);
			}
			else {
				memset(Team_data[currentTeam].ship_count_variables[index], 0, TOKEN_LENGTH);
			}

			Team_data[currentTeam].ship_list[index] = ship.infoIndex;
			strcpy_s(Team_data[currentTeam].ship_list_variables[index], ship.name.c_str());
			index++;
		}

		Team_data[currentTeam].num_ship_choices = index;
		index = 0;

		for (auto& weapon : modelTeam.weapons) {
			Team_data[currentTeam].weaponry_count[index] = weapon.extraAllocated;

			if (weapon.varCountIndex > -1) {
				strcpy_s(Team_data[currentTeam].weaponry_amount_variable[index], Sexp_variables[weapon.varCountIndex].variable_name);
			}
			else {
				memset(Team_data[currentTeam].weaponry_amount_variable[index], 0, TOKEN_LENGTH);
			}

			Team_data[currentTeam].weaponry_pool[index] = weapon.infoIndex;
			memset(Team_data[currentTeam].weaponry_pool_variable[index], 0, TOKEN_LENGTH);
			index++;
		}

		for (auto& weapon : modelTeam.varWeapons) {
			Team_data[currentTeam].weaponry_count[index] = weapon.extraAllocated;

			if (weapon.varCountIndex > -1) {
				strcpy_s(Team_data[currentTeam].weaponry_amount_variable[index], Sexp_variables[weapon.varCountIndex].variable_name);
			}
			else {
				memset(Team_data[currentTeam].weaponry_amount_variable[index], 0, TOKEN_LENGTH);
			}

			Team_data[currentTeam].weaponry_pool[index] = weapon.infoIndex;
			strcpy_s(Team_data[currentTeam].weaponry_pool_variable[index], weapon.name.c_str());
			index++;
		}

		Team_data[currentTeam].num_weapon_choices;
	}
	
	
	return true; 

} 

void LoadoutDialogModel::reject() {
	// just clear out the info we're not going to end up using.
	_shipList.clear();
	_weaponList.clear();
	_shipVarList.clear();
	_weaponVarList.clear();
	_numberVarList.clear();
	_teams.clear();

} // let him go Harry, let him go

void LoadoutDialogModel::buildCurrentLists()
{
	// we're rebuilding the whole list, so we have to clear the text lists out.
	_shipList.clear();
	_weaponList.clear();
	_shipVarList.clear();
	_weaponVarList.clear();

	int index = 0;
	for (auto& item : _teams[_currentTeam].ships) {
		_shipList.emplace_back(createItemString(true, index), item.enabled);
		index++;
	}

	index = 0;

	for (auto& item : _teams[_currentTeam].weapons) {
		_weaponList.emplace_back(createItemString(false, index), item.enabled);
		index++;
	}
	
	for (auto& sexp : Sexp_variables) {
		bool found = false;
		if ((sexp.type & SEXP_VARIABLE_SET) && (sexp.type & SEXP_VARIABLE_STRING)){
			for (auto& item : _teams[_currentTeam].varShips) {
				if (item.name == sexp.variable_name) {
					found = true;
					break;
				}
			}

			_shipVarList.emplace_back(sexp.variable_name, found);

			found = false;

			for (auto& item : _teams[_currentTeam].varWeapons) {
				if (item.name == sexp.variable_name) {
					found = true;
					break;
				}
			}

			_weaponVarList.emplace_back(sexp.variable_name, found);
		}
	}
}


}
}
}