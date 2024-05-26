#include "AbstractDialogModel.h"
#include "globalincs/pstypes.h"

namespace fso {
namespace fred {
namespace dialogs {

class WingEditorDialogModel : public AbstractDialogModel {
public:
	WingEditorDialogModel(QObject* parent, EditorViewport* viewport);

	// The model in this dialog directly applies changes to the mission, so apply and reject are superfluous	
	bool apply() { return true; }
	void reject() {}

	SCP_string getCurrentWingName();
	SCP_vector<SCP_string> getCurrentSelectableWings();
	
	SCP_string getLeader();
	SCP_vector<SCP_string> getLeaderList();
	int getWaveCount();
	int getWaveThreshhold();
	SCP_string getHotkey();
	SCP_string getHotkeyList();
	bool getReinforcementFlag();
	bool getCountingGoalsFlag();
	bool getArrivalMusicFlag();
	bool getArrivalMessageFlag();
	bool getFirstWaveMessageFlag();
	bool getDynamicGoalsFlag();
	int getArrivalType();
	SCP_string getArrivalTarget();
	SCP_vector<SCP_string> getArrivalTargetList();
	int getArrivalDistance();
	int getInitialDelay();
	int getMinWaveDelay();
	int getMaxWaveDelay();
	int getDepartureType();
	SCP_string getDepartureTarget();
	SCP_vector<SCP_string> getDepartureTargetList();
	int getPredepartureDelay();

	SCP_string switchCurrentWing(SCP_string name);
	void switchToNextWing();
	void switchToPreviousWing();

	SCP_string renameWing(SCP_string newName);

	void deleteWing();
	void disbandWing();
	int setLeader(int newLeaderIndex);
	int setTotalWaves(int newTotalWaves);
	int setWaveThreshhold(int newThreshhold);
	int setHotKey(int newHotkeyIndex);
	SCP_string setSquadLogo(SCP_string filname);
	bool setReinforcementFlag(bool flagIn);
	bool setCountingGoalsFlag(bool flagIn);
	bool setArrivalMusicFlag(bool flagIn);
	bool setArrivalMessageFlag(bool flagIn);
	bool setFirstWaveMessageFlag(bool flagIn);
	bool setDynamicGoalsFlag(bool flagIn);
	int setArrivalType(int arrivalType);
	int setInitialArrivalDelay(int delayIn);
	int setArrivalTarget(int targetIndex);
	int setArrivalDistance(int newDistance);
	int setMinWingDelay(int newMin);
	int setMaxWingDelay(int newMax);
	int setDepartureType(int departureType);
	int setDepartureTarget(int targetIndex);
	int setPreDepartureDelay(int newDelay);

private:
	int _currentWingIndex;
	SCP_string _currentWingName;
};


}
}
}


