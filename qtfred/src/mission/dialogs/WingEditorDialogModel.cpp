#include "WingEditorDialogModel.h"
#include <QMessageBox>
#include "FredApplication.h"
#include <unordered_set>

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

std::pair<int, SCP_vector<SCP_string>> WingEditorDialogModel::getLeaderList(){}
int WingEditorDialogModel::getWaveCount() 
{ 
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].wave_count;
}

int WingEditorDialogModel::getWaveThreshhold()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].threshold;
}

std::pair<int, SCP_vector<SCP_string>> WingEditorDialogModel::getHotkeyList(){}
bool WingEditorDialogModel::getReinforcementFlag(){}
bool WingEditorDialogModel::getCountingGoalsFlag(){}
bool WingEditorDialogModel::getArrivalMusicFlag(){}
bool WingEditorDialogModel::getArrivalMessageFlag(){}
bool WingEditorDialogModel::getFirstWaveMessageFlag(){}
bool WingEditorDialogModel::getDynamicGoalsFlag(){}
int WingEditorDialogModel::getArrivalType(){}
std::pair<int,SCP_vector<SCP_string>> WingEditorDialogModel::getArrivalTargetList(){}
int WingEditorDialogModel::getArrivalDistance(){}

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
std::pair<int, SCP_vector<SCP_string>> WingEditorDialogModel::getDepartureTargetList(){}

int WingEditorDialogModel::getPredepartureDelay()
{
	if (_currentWingIndex < 0 || _currentWingIndex > MAX_WINGS) 
		return -1; 
	
	return Wings[_currentWingIndex].departure_delay;
}


} // dialogs
} // fred
} // fso