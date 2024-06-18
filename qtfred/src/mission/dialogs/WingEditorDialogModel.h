#include "AbstractDialogModel.h"
#include "globalincs/pstypes.h"
#include <utility>

namespace fso {
namespace fred {
namespace dialogs {

constexpr int NUM_HOTKEYS = 8;
constexpr int NUM_ARRIVAL_TYPES = 8;
constexpr int NUM_DEPARTURE_TYPES = 2;

class WingEditorDialogModel : public AbstractDialogModel {
public:
	WingEditorDialogModel(QObject* parent, EditorViewport* viewport);

	// The model in this dialog directly applies changes to the mission, so apply and reject are superfluous	
	bool apply() { return true; }
	void reject() {}

	SCP_string getCurrentWingName() { return _currentWingName; }
	SCP_vector<SCP_string> getCurrentSelectableWings();

	std::pair<int, SCP_vector<SCP_string>> getLeaderList();
	int getNumberOfWaves();
	int getWaveThreshhold();
	int getHotkey();
	bool getReinforcementFlag();
	bool getCountingGoalsFlag();
	bool getArrivalMusicFlag();
	bool getArrivalMessageFlag();
	bool getFirstWaveMessageFlag();
	bool getDynamicGoalsFlag();
	bool getNoArrivalWarpFlag();
	bool getNoDepartureWarpFlag();
	bool getSameArrivalWarpWhenDockedFlag();
	bool getSameDepartureWarpWhenDockedFlag();
	SCP_vector<std::pair<string, bool>> getArrivalPathStatus();
	SCP_vector<std::pair<string, bool>> getDeparturePathStatus();

	int getArrivalType();
	std::pair<int, SCP_vector<SCP_string>> getArrivalTargetList();
	int getArrivalDistance();
	int getInitialDelay();
	int getMinWaveDelay();
	int getMaxWaveDelay();
	int getDepartureType();
	std::pair<int, SCP_vector<SCP_string>> getDepartureTargetList();
	int getPredepartureDelay();
	SCP_string getSquadronLogo();

	SCP_string switchCurrentWing(SCP_string name);
	SCP_string switchToNextWing();
	SCP_string switchToPreviousWing();

	SCP_string renameWing(SCP_string newName);

	void deleteWing();
	void disbandWing();
	int setLeader(int newLeaderIndex);
	int setTotalWaves(int newTotalWaves);
	int setWaveThreshhold(int newThreshhold);
	int setHotKey(int newHotkeyIndex);
	SCP_string setSquadLogo(SCP_string filename);
	bool setReinforcementFlag(bool flagIn);
	bool setCountingGoalsFlag(bool flagIn);
	bool setArrivalMusicFlag(bool flagIn);
	bool setArrivalMessageFlag(bool flagIn);
	bool setFirstWaveMessageFlag(bool flagIn);
	bool setDynamicGoalsFlag(bool flagIn);
	bool setNoArrivalWarpFlag(bool flagIn);
	bool setNoDepartureWarpFlag(bool flagIn);
	bool setSameArrivalWarpWhenDockedFlag(bool flagIn);
	bool setSameDepartureWarpWhenDockedFlag(bool flagIn);
	int setArrivalType(int arrivalType);
	int setInitialArrivalDelay(int delayIn);
	int setArrivalTarget(int targetIndex);
	int setArrivalDistance(int newDistance);
	bool setArrivalPath(std::pair<int, bool> pathStatusIn);
	bool resetArrivalPaths();
	int setMinWingDelay(int newMin);
	int setMaxWingDelay(int newMax);
	int setDepartureType(int departureType);
	int setDepartureTarget(int targetIndex);
	int setPreDepartureDelay(int newDelay);
	bool resetDeparturePaths();
	bool setDeparturePath(std::pair<int, bool> pathStatusIn);

//arrival_path_mask
//departure_path_mask

private:
	int _currentWingIndex;
	SCP_string _currentWingName;

	void populateCurrentSelection();
};


}
}
}


