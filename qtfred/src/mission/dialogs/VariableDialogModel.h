#pragma once

#include "globalincs/pstypes.h"

#include "AbstractDialogModel.h"
#include <QMessageBox>

namespace fso {
namespace fred {
namespace dialogs {

struct variableInfo {
	SCP_string name = "<unnamed>";
	SCP_string originalName = "";
	bool deleted = false;
	bool string = true;
	int flags = 0;
	int numberValue = 0;
	SCP_string stringValue = "";
};


struct containerInfo {
	SCP_string name = "<unnamed>";
	bool deleted = false;
	bool list = true;
	bool string = true;
	bool integerKeys = false;
	int flags = 0;

	// this will allow us to look up the original values used in the mission previously.
	SCP_string originalName = "";

	// I found out that keys could be strictly typed as numbers *after* finishing the majority of the model....
	// So I am just going to store numerical keys as strings and use a bool to differentiate. 
	SCP_vector<SCP_string> keys;
	SCP_vector<int> numberValues;
	SCP_vector<SCP_string> stringValues;
};

class VariableDialogModel : public AbstractDialogModel {
public:
	VariableDialogModel(QObject* parent, EditorViewport* viewport);

	// true on string, false on number
	bool getVariableType(int index);
	bool getVariableNetworkStatus(int index);
	// 0 neither, 1 on mission complete, 2 on mission close (higher number saves more often)
	int getVariableOnMissionCloseOrCompleteFlag(int index);
	bool getVariableEternalFlag(int index);

	SCP_string getVariableStringValue(int index);
	int getVariableNumberValue(int index);

	// !! Note an innovation: when getting a request to set a value, 
	// this model will return the value that sticks and then will overwrite
	// the value in the dialog.  This means that we don't have to have the UI 
	// repopulate the whole editor on each change.

	// true on string, false on number
	bool setVariableType(int index, bool string);
	bool setVariableNetworkStatus(int index, bool network);
	int setVariableOnMissionCloseOrCompleteFlag(int index, int flags);
	bool setVariableEternalFlag(int index, bool eternal);

	SCP_string setVariableStringValue(int index, SCP_string value);
	int setVariableNumberValue(int index, int value);

	SCP_string addNewVariable();
	SCP_string changeVariableName(int index, SCP_string newName);
	SCP_string copyVariable(int index);
	// returns whether it succeeded
	bool removeVariable(int index);

	// Container Section

	// true on string, false on number
	bool getContainerValueType(int index);
	// true on list, false on map
	bool getContainerListOrMap(int index);
	bool getContainerNetworkStatus(int index);
	// 0 neither, 1 on mission complete, 2 on mission close (higher number saves more often)
	int getContainerOnMissionCloseOrCompleteFlag(int index);
	bool getContainerEternalFlag(int index);

	bool setContainerValueType(int index, bool type);
	bool setContainerKeyType(int index, bool string);
	bool setContainerListOrMap(int index, bool list);
	bool setContainerNetworkStatus(int index, bool network);
	int setContainerOnMissionCloseOrCompleteFlag(int index, int flags);
	bool setContainerEternalFlag(int index, bool eternal);

	SCP_string addContainer();
	SCP_string addContainer(SCP_string nameIn);
	SCP_string copyContainer(int index);
	SCP_string changeContainerName(int index, SCP_string newName);
	bool removeContainer(int index);

	SCP_string addListItem(int index);
	SCP_string copyListItem(int containerIndex, int index);
	bool removeListItem(int containerindex, int index);

	std::pair<SCP_string, SCP_string> addMapItem(int index);
	std::pair<SCP_string, SCP_string> copyMapItem(int index, int itemIndex);
	bool removeMapItem(int index, int rowIndex);

	SCP_string replaceMapItemKey(int index, SCP_string oldKey, SCP_string newKey);
	SCP_string changeMapItemStringValue(int index, SCP_string key, SCP_string newValue);
	SCP_string changeMapItemNumberValue(int index, SCP_string key, int newValue);
	
	const SCP_vector<SCP_string>& getMapKeys(int index);
	const SCP_vector<SCP_string>& getStringValues(int index);
	const SCP_vector<int>& getNumberValues(int index);

	const SCP_vector<std::array<SCP_string, 3>> getVariableValues();
	const SCP_vector<std::array<SCP_string, 3>> getContainerNames();
	void checkValidModel();

	bool apply() override;
	void reject() override;

	void initializeData();

	static SCP_string trimNumberString(SCP_string source);

private:
	SCP_vector<variableInfo> _variableItems;
	SCP_vector<containerInfo> _containerItems;
	int _listTextMode = 0;
	int _mapTextMode = 0;

	variableInfo* lookupVariable(int index){
		if(index > -1 &&  index < static_cast<int>(_variableItems.size()) ){
			return &_variableItems[index];	
		}

		return nullptr;
	}

	variableInfo* lookupVariableByName(SCP_string name){
		for (int x = 0; x < static_cast<int>(_variableItems.size()); ++x) {
			if (_variableItems[x].name == name) {
				return &_variableItems[x];
			}
		}

		return nullptr;
	}

	containerInfo* lookupContainer(int index){
		if(index > -1 &&  index < static_cast<int>(_containerItems.size()) ){
			return &_containerItems[index];	
		}
		
		return nullptr;
	}

	containerInfo* lookupContainerByName(SCP_string name){
		for (int x = 0; x < static_cast<int>(_containerItems.size()); ++x) {
			if (_containerItems[x].name == name) {
				return &_containerItems[x];
			}
		}

		return nullptr;
	}


	// many of the controls in this editor can lead to drastic actions, so this will be very useful.
	bool confirmAction(SCP_string question, SCP_string informativeText)
	{
	QMessageBox msgBox;
	msgBox.setText(question.c_str());
	msgBox.setInformativeText(informativeText.c_str());
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	int ret = msgBox.exec();

	switch (ret) {
		case QMessageBox::Yes:
			return true;
			break;
		case QMessageBox::Cancel:
			return false;
			break;
		default:
			UNREACHABLE("Bad return value from confirmation message box in the Loadout dialog editor.");
			break;
		}
	}

};

} // namespace dialogs
} // namespace fred
} // namespace fso
