#include "WingEditorDialog.h"
#include <ui_WingEditorDialog.h>


namespace fso {
namespace fred {
namespace dialogs {

const std::array<SCP_string, NUM_HOTKEYS + 1> HOTKEY_UI_LIST{
	"None",
	"Set 1 (F5)",
	"Set 2 (F6)",
	"Set 3 (F7)",
	"Set 4 (F8)",
	"Set 5 (F9)",
	"Set 6 (F10)",
	"Set 7 (F11)",
	"Set 8 (F12)"
}

const std::array<SCP_string, NUM_ARRIVAL_TYPES + 1> ARRIVAL_UI_LIST{
	"None",
	"Temp 1",
	"Temp 2",
	"Temp 3",
	"Temp 4",
	"Temp 5",
	"Temp 6"
}

const std::array<SCP_string, NUM_DEPARTURE_TYPES + 1> DEPARTURE_UI_LIST{
	"None",
	"Temp 1",
	"Temp 2"
}


WingEditorDialog::WingEditorDialog(FredView* parent, EditorViewport* viewport) 
	: QDialog(parent), ui(new Ui::WingEditorDialog()), _model(new WingEditorDialogModel(this, viewport)), _viewport(viewport)
{
	this->setFocus();
	ui->setupUi(this);
	resize(QDialog::sizeHint()); // The best I can tell without some research, when a dialog doesn't use an underlying grid or layout, it needs to be resized this way before anything will show up 

	// apply and reject do not ... uh ... apply here because changes are in real time.

	// CONNECT STATEMENTS GO HERE


	// Set up wing list
	// TODO! This needs a signal to be updated whenever a wing is created or destroyed.
	auto wingList = _model->getCurrentSelectableWings();

	for (const auto& wing : wingList){
		ui->selectWingCombobox->addItem(wing.c_str());
	}

	// Set up the hotkey Combobox
	for (const auto& string : HOTKEY_UI_LIST){
		ui->hotkeyComboBox->addItem(string.c_str());
	}

	ui->hotkeyComboBox->setCurrentIndex(0);

	// Set up saving the sizes of the UI elements so that we can expand and contract at will.

}

WingEditorDialog::loadWing() 
{
	auto wingName = _model->getCurrentWingName();

	// Shut everything down because there is no wing.
	if (wingName.empty()){
		ui->wingNameLineEdit->clear();
		ui->wingNameLineEdit->setEnabled(false);

		ui->totalWavesSpinBox->clear();
		ui->totalWavesSpinBox->setEnabled(false);

		ui->hotkeyComboBox->setCurrentIndex(0);
		ui->hotkeyComboBox->setEnabled(false);

		ui->wingLeaderComboBox->clear();
		ui->wingLeaderComboBox->setEnabled(false);

		ui->waveThresholdSpinBox->clear();
		ui->waveThresholdSpinBox->setEnabled(false);

		ui->squadLogoLineEdit->clear();
		ui->squadLogoLineEdit->setEnabled(false);

		ui->reinforcementUnitFlagCheckbox->setChecked(false);
		ui->reinforcementUnitFlagCheckbox->setEnabled(false);

		ui->ingoreCountingGoalsFlagCheckbox->setChecked(false);
		ui->ingoreCountingGoalsFlagCheckbox->setEnabled(false);

		ui->noArrivalMusicFlagCheckbox->setChecked(false);
		ui->noArrivalMusicFlagCheckbox->setEnabled(false);

		ui->noArrivalMessageFlagCheckbox->setChecked(false);
		ui->noArrivalMessageFlagCheckbox->setEnabled(false);

		ui->noFirstWaveMessageCheckbox->setChecked(false);
		ui->noFirstWaveMessageCheckbox->setEnabled(false);

		ui->noDynamicGoalsCheckbox->setChecked(false);
		ui->noDynamicGoalsCheckbox->setEnabled(false);

		// warp parameters that don't have ui elements yet.
		/*
		ui->noArrivalWarpCheckbox->setChecked(_model->getNoArrivalWarpFlag());
		ui->noArrivalWarpCheckbox->setEnabled(true);

		ui->noDepartureWarpCheckbox->setChecked(_model->getNoDepartureWarpFlag());
		ui->noDepartureWarpCheckbox->setEnabled(true);

		ui->sameArrivalWarpWhenDockedCheckbox->setChecked(_model->getSameArrivalWarpWhenDockedFlag());
		ui->sameArrivalWarpWhenDockedCheckbox->setEnabled(true);

		ui->sameDepartureWarpWhenDockedCheckbox->setChecked(_model->getSameDepartureWarpWhenDockedFlag());
		ui->sameDepartureWarpWhenDockedCheckbox->setEnabled(true);
		*/

	// Display all info based on the wing selected in the model
	} else {

		ui->wingNameLineEdit->setEnabled(true);
		ui->wingNameLineEdit->setText(wingName.c_str());

		ui->totalWavesSpinBox->setEnabled(true);
		ui->totalWavesSpinBox->setValue(_model->getNumberOfWaves());

		ui->hotkeyComboBox->setEnabled(true);
		ui->hotkeyComboBox->setCurrentIndex(_model->getHotkey() + 1); // Combo box thinks that "none" is index 0, so adjust

		auto leaderList _model->getLeaderList();
		ui->wingLeaderComboBox->setEnabled(true);
		ui->wingLeaderComboBox->clear();

		for (const auto& leader : leaderList.second){
			ui->wingLeaderComboBox->addItem(leader.c_str());
		}

		ui->wingLeaderComboBox->setCurrentIndex(leaderList.first);

		ui->waveThresholdSpinBox->setEnabled(true);
		ui->waveThresholdSpinBox->setValue(_model->getWaveThreshhold());

		ui->squadLogoLineEdit->setEnabled(true);
		ui->squadLogoLineEdit->setText(_model->getSquadronLogo().c_str());

		ui->reinforcementUnitFlagCheckbox->setEnabled(true);
		ui->reinforcementUnitFlagCheckbox->setChecked(_model->getReinforcementFlag());

		ui->ingoreCountingGoalsFlagCheckbox->setEnabled(true);
		ui->ingoreCountingGoalsFlagCheckbox->setChecked(getCountingGoalsFlag());

		ui->noArrivalMusicFlagCheckbox->setEnabled(true);
		ui->noArrivalMusicFlagCheckbox->setChecked(_model->getArrivalMusicFlag());

		ui->noArrivalMessageFlagCheckbox->setEnabled(true);
		ui->noArrivalMessageFlagCheckbox->setChecked(_model->getArrivalMessageFlag());

		ui->noFirstWaveMessageCheckbox->setEnabled(true);
		ui->noFirstWaveMessageCheckbox->setChecked(_model->getFirstWaveMessageFlag());

		ui->noDynamicGoalsCheckbox->setEnabled(true);
		ui->noDynamicGoalsCheckbox->setChecked(_model->getDynamicGoalsFlag());

		// warp parameters that don't have ui elements yet.
		/*
		ui->noArrivalWarpCheckbox->setEnabled(true);
		ui->noArrivalWarpCheckbox->setChecked(_model->getNoArrivalWarpFlag());

		ui->noDepartureWarpCheckbox->setEnabled(true);
		ui->noDepartureWarpCheckbox->setChecked(_model->getNoDepartureWarpFlag());

		ui->sameArrivalWarpWhenDockedCheckbox->setEnabled(true);
		ui->sameArrivalWarpWhenDockedCheckbox->setChecked(_model->getSameArrivalWarpWhenDockedFlag());

		ui->sameDepartureWarpWhenDockedCheckbox->setEnabled(true);
		ui->sameDepartureWarpWhenDockedCheckbox->setChecked(_model->getSameDepartureWarpWhenDockedFlag());
		*/

		int ()
		ui->arrivalTypeComboBox->setCurrentIndex(_model->getArrivalType())
		/*
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
	// No UI for these yet
	bool getNoArrivalWarpFlag();
	bool getNoDepartureWarpFlag();
	bool getSameArrivalWarpWhenDockedFlag();
	bool getSameDepartureWarpWhenDockedFlag();


	int getArrivalType();
	std::pair<int, SCP_vector<SCP_string>> getArrivalTargetList();
	int getArrivalDistance();
	int getInitialDelay();
	int getMinWaveDelay();
	int getMaxWaveDelay();
	int getDepartureType();
	std::pair<int, SCP_vector<SCP_string>> getDepartureTargetList();
	int getPredepartureDelay();
	SCP_string getSquadronLogo();*/

	
	}
	/*
	




arrivalTypeComboBox
departureTypeLabel
departureTypeComboBox
departureTargetLabel
departureTargetComboBox
initialArrivalDelayLabel
initialArrivalDelaySpinBox
arrivalTargetHelperLabel
arrivalDistanceHelperLabel
preDepartureDelayLabel
preDepartureDelaySpinBox
delayBetweenWavesGroupBox
minWaveDelayLabel
minWaveDelayLineEdit
maxWaveDelayLabel
maxWaveDelayLineEdit
*/
} 


WingEditorDialog::~WingEditorDialog() {}; //NOLINT

} // dialogs
} // fred
} // fso