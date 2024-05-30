#include "WingEditorDialogModel.h"
#include <QMessageBox>
#include "FredApplication.h"
#include <unordered_set>
#include "mission/missionparse.h"

namespace fso {
namespace fred {
namespace dialogs {


WingEditorDialogModel::WingEditorDialogModel(QObject* parent, EditorViewport* viewport) 
		: AbstractDialogModel(parent, viewport)
{
	populateCurrentSelection();
}

void WingEditorDialogModel::populateCurrentSelection()
{
	object* objp = GET_FIRST(&obj_used_list);

	SCP_string candidateName;
	int lastIndex = -1;

	while (objp != END_OF_LIST(&obj_used_list)) {

		// do we have a marked ship?
		if ((objp->type == OBJ_SHIP || objp->type == OBJ_START) && objp->flags[Object::Object_Flags::Marked]) {

			// do we have a valid instance from that marked ship?
			if (objp->instance > -1 && objp->instance < MAX_SHIPS){

				// is that ship in a wing?
				if (Ships[objp->instance].wingnum > -1 && Ships[objp->instance].wingnum < MAX_WINGS){

					// First wing ship?
					if (lastIndex == -1){
						lastIndex = Ships[objp->instance].wingnum;

					// did this match the previous wing we found?
					} else if (lastIndex != Ships[objp->instance].wingnum) {
						// remove the value from last index because we didn't find a wing we could use.
						lastIndex = -1;
						break;
					}
				}
			}
		}
	}

	if (lastIndex < 0 || lastIndex >= MAX_WINGS){
		_currentWingIndex = -1;
		_currentWingName = "";
	} else {
		_currentWingIndex = lastIndex;
		_currentWingName = Wings[lastIndex].name;
	}
}

SCP_vector<SCP_string> WingEditorDialogModel::getCurrentSelectableWings()
{
	// going to make an exception for this function.  Going to just send the list as a secondary thing that gets added to the rest of the combobox
	SCP_vector<SCP_string> wingNames;

	for (const auto& wing : Wings){
		// strlen is slow.... and that's all I was trying to check anyway
		if (wing.name[0] != '\0'){
			wingNames.emplace_back(wing.name);
		}
	}

	return wingNames;
}

std::pair<int, SCP_vector<SCP_string>> WingEditorDialogModel::getLeaderList()
{
	std::pair<int, SCP_vector<SCP_string>> out;

	out.first = Wings[_currentWingIndex].special_ship;

	for (int x = 0; Wings[i].wave_count > MAX_SHIPS_PER_WING; ++x){
		if (Wings[_currentWingIndex].ship_index[x] > -1 && Wings[_currentWingIndex].ship_index[x] < MAX_SHIPS){
			out.second.emplace_back(Ships[Wings[_currentWingIndex].ship_index[x]].ship_name);
		}
	}

	return out;
}

int WingEditorDialogModel::getNumberOfWaves() 
{ 
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].num_waves;
}

int WingEditorDialogModel::getWaveThreshhold()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].threshold;
}

int WingEditorDialogModel::getHotkey()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].hotkey;
}

bool WingEditorDialogModel::getReinforcementFlag()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].flags[Ship::Wing_Flags::Reinforcement];
}

bool WingEditorDialogModel::getCountingGoalsFlag()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].flags[Ship::Wing_Flags::Ignore_count];
}

bool WingEditorDialogModel::getArrivalMusicFlag()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].flags[Ship::Wing_Flags::No_arrival_music];
}

bool WingEditorDialogModel::getArrivalMessageFlag()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].flags[Ship::Wing_Flags::No_arrival_message];
}

bool WingEditorDialogModel::getFirstWaveMessageFlag()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].flags[Ship::Wing_Flags::No_first_wave_message];
}


bool WingEditorDialogModel::getDynamicGoalsFlag()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].flags[Ship::Wing_Flags::No_dynamic];
}

int WingEditorDialogModel::getArrivalType()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	return Wings[_currentWingIndex].arrival_location;
}

// TODO, This one is going to be complicated because we need to have different target lists
// depending on the mode.
std::pair<int,SCP_vector<SCP_string>> WingEditorDialogModel::getArrivalTargetList()
{

}

int WingEditorDialogModel::getArrivalDistance()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	return Wings[_currentWingIndex].arrival_distance;
}

int WingEditorDialogModel::getInitialDelay()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].arrival_delay;
}

int WingEditorDialogModel::getMinWaveDelay()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].wave_delay_min;
}

int WingEditorDialogModel::getMaxWaveDelay()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].wave_delay_max;
}

int WingEditorDialogModel::getDepartureType()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].departure_location;
}

std::pair<int, SCP_vector<SCP_string>> WingEditorDialogModel::getDepartureTargetList()
{
	
}

int WingEditorDialogModel::getPredepartureDelay()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].departure_delay;
}

SCP_string WingEditorDialogModel::switchCurrentWing(SCP_string name)
{
	for (int x = 0; x < MAX_WINGS; ++x){
		SCP_string temp = Wings[x].name;

		if (temp == name){
			_currentWingIndex = x;
			_currentWingName = temp;
			return temp;
		}
	}

	_currentWingIndex = -1;
	_currentWingName = "";
	return _currentWingName;
}

SCP_string WingEditorDialogModel::switchToNextWing()
{
	int startIndex = _currentWingIndex;

	if (startIndex < 0 || startIndex >= MAX_WINGS){
		startIndex = 0;
	}

	for (int x = startIndex + 1; x < MAX_WINGS; ++x){
		SCP_string temp = Wings[x].name;

		// Any deleted wing will have an empty name.
		if (!temp.empty()){
			_currentWingIndex = x;
			_currentWingName = temp;
			return temp;
		}
	}

	// if we started at -1 or an invalid value and didn't find anything, return
	if (_currentWingIndex < 0 || _currentWingIndex >= MAX_WINGS){
		return "";
	}

	// if we need to handle wrap, continue.  Not a typo! If we didn't find another wing,
	// just reselect the same wing.
	for (int x = 0; x <= _currentWingIndex; ++x){
		SCP_string temp = Wings[x].name;

		// Any deleted wing will have an empty name.
		if (!temp.empty()){
			_currentWingIndex = x;
			_currentWingName = temp;
			return temp;
		}
	}

	// No wings somehow
	_currentWingIndex = x;
	_currentWingName = temp;
	return _currentWingName;
}

SCP_string WingEditorDialogModel::switchToPreviousWing()
{
	int startIndex = _currentWingIndex - 1;

	if (startIndex < 0 || startIndex >= MAX_WINGS){
		startIndex = MAX_WINGS - 1;
	}

	for (int x = startIndex; x > -1; --x){
		SCP_string temp = Wings[x].name;

		// Any deleted wing will have an empty name.
		if (!temp.empty()){
			_currentWingIndex = x;
			_currentWingName = temp;
			return temp;
		}
	}

	// if we started at -1 or an invalid value and didn't find anything, return
	if (_currentWingIndex < 0 || _currentWingIndex >= MAX_WINGS){
		return "";
	}

	// if we need to handle wrap, continue.  Not a typo! If we didn't find another wing,
	// just reselect the same wing.
	for (int x = MAX_WINGS - 1; x >= _currentWingIndex; --x){
		SCP_string temp = Wings[x].name;

		// Any deleted wing will have an empty name.
		if (!temp.empty()){
			_currentWingIndex = x;
			_currentWingName = temp;
			return temp;
		}
	}

	// No wings somehow
	_currentWingIndex = -1;
	_currentWingName = "";
	return _currentWingName;
}

int WingEditorDialogModel::setLeader(int newLeaderIndex)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS){
		return -1; 
	}
	
	if (newLeaderIndex < 0 || newLeaderIndex >= Wings[_currentWingIndex].wave_count){
		return Wings[_currentWingIndex].special_ship;
	}

	Wings[_currentWingIndex].special_ship = newLeaderIndex;
	return Wings[_currentWingIndex].special_ship;
}

int WingEditorDialogModel::setTotalWaves(int newTotalWaves) 
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	// you read that right, I don't see a limit for the number of waves.
	// Original Fred had a UI limit of 99, but lol
	if (newTotalWaves < 1){
		return Wings[currentWingIndex].num_waves;
	}

	Wings[currentWingIndex].num_waves = newTotalWaves;
	return Wings[currentWingIndex].num_waves;
}

// TODO - Is there any other explicit or implicit limit for the wave threshold.  For instance, Fred only allows
// the threshold to be less than the wing size.
int WingEditorDialogModel::setWaveThreshhold(int newThreshold)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	if (newThreshold < 0){
		Wings[_currentWingIndex].threshold = 0;
		return Wings[_currentWingIndex].threshold;
	}
	
	// Can this be higher than the number of ships in a wing?
	if (newThreshold + Wings[_currentWingIndex].wave_count > MAX_SHIPS_PER_WING){
		Wings[_currentWingIndex].threshold = MAX_SHIPS_PER_WING - Wings[_currentWingIndex].wave_count;
		return Wings[_currentWingIndex].threshold;
	}

	Wings[_currentWingIndex].threshold = newThreshold;
	return Wings[_currentWingIndex].threshold;
}

int WingEditorDialogModel::setMaxWingDelay(int newMax)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	if (newMax < Wings[_currentWingIndex].wave_delay_min){
		Wings[_currentWingIndex].wave_delay_max = Wings[_currentWingIndex].wave_delay_min;
		return 	Wings[_currentWingIndex].wave_delay_max;
	}

	// Angels in the Outfield kid: "It could happen."
	if (newMax < 0) {
		return 	Wings[_currentWingIndex].wave_delay_max;
	}

	Wings[_currentWingIndex].wave_delay_max = newMax;
	return newMax;	
}

int WingEditorDialogModel::setMinWingDelay(int newMin)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	if (newMin > Wings[_currentWingIndex].wave_delay_max) {
		Wings[_currentWingIndex].wave_delay_min = Wings[_currentWingIndex].wave_delay_max;
		return Wings[_currentWingIndex].wave_delay_min;
	}

	if (newMin < 0){
		Wings[_currentWingIndex].wave_delay_min = 0;
		return Wings[_currentWingIndex].wave_delay_min;
	}

	Wings[_currentWingIndex].wave_delay_min = newMin;
}

bool WingEditorDialogModel::setReinforcementFlag(bool flagIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	// TODO: This may need to add/remove the reinforcement flags for the inidividual ships.
	if (flagIn){
		Wings[currentWingIndex].flags.set(Ship::Wing_Flags::Reinforcement);
	} else {
		Wings[currentWingIndex].flags.remove(Ship::Wing_Flags::Reinforcement);
	}

	return Wings[currentWingIndex].flags[Ship::Wing_Flags::Reinforcement];
}

bool WingEditorDialogModel::setCountingGoalsFlag(bool flagIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	if (flagIn){
		Wings[currentWingIndex].flags.set(Ship::Wing_Flags::Ignore_count);
	} else {
		Wings[currentWingIndex].flags.remove(Ship::Wing_Flags::Ignore_count);
	}


	return Wings[currentWingIndex].flags[Ship::Wing_Flags::Ignore_count];
}

bool WingEditorDialogModel::setArrivalMusicFlag(bool flagIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	if (flagIn){
		Wings[currentWingIndex].flags.set(Ship::Wing_Flags::No_arrival_music);
	} else {
		Wings[currentWingIndex].flags.remove(Ship::Wing_Flags::No_arrival_music);
	}

	return Wings[currentWingIndex].flags[Ship::Wing_Flags::No_arrival_music];
}

bool WingEditorDialogModel::setArrivalMessageFlag(bool flagIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	if (flagIn){
		Wings[currentWingIndex].flags.set(Ship::Wing_Flags::No_arrival_message);
	} else {
		Wings[currentWingIndex].flags.remove(Ship::Wing_Flags::No_arrival_message);
	}

	return Wings[currentWingIndex].flags[Ship::Wing_Flags::No_arrival_message];
}

bool WingEditorDialogModel::setFirstWaveMessageFlag(bool flagIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	if (flagIn){
		Wings[currentWingIndex].flags.set(Ship::Wing_Flags::No_first_wave_message);
	} else {
		Wings[currentWingIndex].flags.remove(Ship::Wing_Flags::No_first_wave_message);
	}

	return Wings[currentWingIndex].flags[Ship::Wing_Flags::No_first_wave_message];
}

bool WingEditorDialogModel::setDynamicGoalsFlag(bool flagIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return false; 

	if (flagIn){
		Wings[currentWingIndex].flags.set(Ship::Wing_Flags::No_dynamic);
	} else {
		Wings[currentWingIndex].flags.remove(Ship::Wing_Flags::No_dynamic);
	}

	return Wings[currentWingIndex].flags[Ship::Wing_Flags::No_dynamic];
}

int WingEditorDialogModel::setArrivalDistance(int newDistance)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	if (newDistance < 0){
		Wings[currentWingIndex].arrival_distance = 0;
		return Wings[currentWingIndex].arrival_distance;
	}

	Wings[currentWingIndex].arrival_distance = newDistance;
	return Wings[currentWingIndex].arrival_distance;
}

/*

//		these are going to go in the arrival/departure parameters tab
		m_no_arrival_warp = Wings[cur_wing].flags[Ship::Wing_Flags::No_arrival_warp] ? TRUE : FALSE;
		m_no_departure_warp = Wings[cur_wing].flags[Ship::Wing_Flags::No_departure_warp] ? TRUE : FALSE;
		m_same_arrival_warp_when_docked = Wings[cur_wing].flags[Ship::Wing_Flags::Same_arrival_warp_when_docked] ? TRUE : FALSE;
		m_same_departure_warp_when_docked = Wings[cur_wing].flags[Ship::Wing_Flags::Same_departure_warp_when_docked] ? TRUE : FALSE;

*/


int WingEditorDialogModel::setArrivalType(int arrivalType)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	// if we have an invalid option 
	if (arrivalType < ARRIVE_AT_LOCATION || arrivalType > ARRIVE_FROM_DOCK_BAY){
		Wings[currentWingIndex].arrival_location = ARRIVE_AT_LOCATION;
		return Wings[currentWingIndex].arrival_location;
	}

	Wings[currentWingIndex].arrival_location = arrivalType;
	return Wings[currentWingIndex].arrival_location;
}

int WingEditorDialogModel::setInitialArrivalDelay(int delayIn)
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 

	if (delayIn < 0){
		Wings[currentWingIndex].arrival_delay = 0;
		return Wings[currentWingIndex].arrival_delay;
	}

	Wings[currentWingIndex].arrival_delay = delayIn;
	return Wings[currentWingIndex].arrival_delay;
}

/*#define	ARRIVE_AT_LOCATION			0
#define	ARRIVE_NEAR_SHIP				1
#define	ARRIVE_IN_FRONT_OF_SHIP		2
#define	ARRIVE_FROM_DOCK_BAY			3

// defines for departure locations.  These defines should match their counterparts in the departure location
// array
#define MAX_DEPARTURE_NAMES			2
#define DEPART_AT_LOCATION				0
#define DEPART_AT_DOCK_BAY				1
*/

} // dialogs
} // fred
} // fso