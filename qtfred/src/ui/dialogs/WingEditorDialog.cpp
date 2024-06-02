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
};

const std::array<SCP_string, NUM_ARRIVAL_TYPES + 1> ARRIVAL_UI_LIST{
	"", // this is just a placeholder for when the dialog is inactive.
	"Temp 1",
	"Temp 2",
	"Temp 3",
	"Temp 4",
	"Temp 5",
	"Temp 6"
};

const std::array<SCP_string, NUM_DEPARTURE_TYPES + 1> DEPARTURE_UI_LIST{
	"", // this is just a placeholder for when the dialog is inactive
	"Temp 1",
	"Temp 2"
};


WingEditorDialog::WingEditorDialog(FredView* parent, EditorViewport* viewport) 
	: QDialog(parent), ui(new Ui::WingEditorDialog()), _model(new WingEditorDialogModel(this, viewport)), _viewport(viewport)
{
	this->setFocus();
	ui->setupUi(this);
	resize(QDialog::sizeHint()); // The best I can tell without some research, when a dialog doesn't use an underlying grid or layout, it needs to be resized this way before anything will show up 

	// apply and reject do not ... uh ... apply here because changes are in real time.

	// Connect statements for Dialog Expansion and contraction
	connect(ui->toggleGeneralOptionsButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onToggleGeneralOptionsButtonPressed);

	connect(ui->toggleArrivalDepartureButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onToggleArrivalDepartureButtonPressed);

	connect(ui->toggleWingFlagsButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onToggleWingFlagsButtonPressed);


	// buttons
	connect(ui->previousWingButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onPreviousWingButtonPressed);

	connect(ui->nextWingButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onNextWingButtonPressed);

	connect(ui->initialOrdersButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onInitialOrdersButtonPressed);

	connect(ui->deleteWingButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onDeleteWingButtonPressed);

	connect(ui->disbandWingButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onDisbandWingButtonPressed);

	connect(ui->warpArrivalStopSoundBrowseButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpArrivalStopSoundBrowseButtonPressed);

	connect(ui->warpArrivalStopSoundPlayButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpArrivalStopSoundPlayButtonPressed);

	connect(ui->warpArrivalStartSoundBrowseButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpArrivalStartSoundBrowseButtonPressed);

	connect(ui->warpArrivalStartSoundPlayButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpArrivalStartSoundPlayButtonPressed);

	connect(ui->warpArrivalAnimationBrowseButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpArrivalAnimationBrowseButtonPressed);

	connect(ui->warpDepartureStartSoundBrowseButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpDepartureStartSoundBrowseButtonPressed);

	connect(ui->warpDepartureStartSoundPlayButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpDepartureStartSoundPlayButtonPressed);

	connect(ui->warpDepartureStopSoundBrowseButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpDepartureStopSoundBrowseButtonPressed);

	connect(ui->warpDepartureStopSoundPlayButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpDepartureStopSoundPlayButtonPressed);

	connect(ui->warpDepartureAnimationBrowseButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onWarpDepartureAnimationBrowseButtonPressed);

	connect(ui->browseSquadLogosButton, 
		&QPushButton::clicked, 
		this, 
		&WingEditorDialog::onBrowseSquadLogosButtonPressed);


	connect(ui->warpArrivalStopSoundLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWarpArrivalStopSoundLineEditChanged);

	connect(ui->warpArrivalStartSoundLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWarpArrivalStartSoundLineEditChanged);

	connect(ui->warpArrivalAnimationLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWarpArrivalAnimationLineEditChanged);

	connect(ui->warpDepartureStartSoundLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWarpDepartureStartSoundLineEditChanged);

	connect(ui->warpDepartureStopSoundLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWarpDepartureStopSoundLineEditChanged);

	connect(ui->warpDepartureAnimationLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWarpDepartureAnimationLineEdit);

	connect(ui->wingNameLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onWingNameLineEditChanged);

	connect(ui->squadLogoLineEdit,
		static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
		this,
		&WingEditorDialog::onSquadLogoLineEditChanged);


	connect(ui->warpArrivalDecelerationExponenetDoubleSpinbox,
		static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpArrivalDecelerationExponenetDoubleSpinboxUpdated);

	connect(ui->warpDepartureDecelerationExponenetDoubleSpinbox,
		static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpDepartureDecelerationExponenetDoubleSpinboxUpdated);

	connect(ui->arrivalDistanceSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onArrivalDistanceSpinboxUpdated);

	connect(ui->maxWaveDelaySpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onMaxWaveDelaySpinBoxUpdated);

	connect(ui->minWaveDelaySpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onMinWaveDelaySpinBoxUpdated);

	connect(ui->initialArrivalDelaySpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onInitialArrivalDelaySpinBoxUpdated);

	connect(ui->preDepartureDelaySpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onPreDepartureDelaySpinBoxUpdated);

	connect(ui->warpArrivalEngageTimeSpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpArrivalEngageTimeSpinBoxUpdated);

	connect(ui->warpArrivalShipSpeedSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpArrivalShipSpeedSpinboxUpdated);

	connect(ui->shipArrivalWarpingTimeSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onShipArrivalWarpingTimeSpinboxUpdated);

	connect(ui->warpArrivalRadiusSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpArrivalRadiusSpinboxUpdated);

	connect(ui->warpDepartureEngageTimeSpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpDepartureEngageTimeSpinBoxUpdated);

	connect(ui->shipDepartureWarpingTimeSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onShipDepartureWarpingTimeSpinboxUpdated);

	connect(ui->warpDepartureRadiusSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWarpDepartureRadiusSpinboxUpdated);

	connect(ui->totalWavesSpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onTotalWavesSpinBoxUpdated);

	connect(ui->waveThresholdSpinBox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&WingEditorDialog::onWaveThresholdSpinBoxUpdated);



	connect(ui->reinforcementUnitFlagCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onReinforcementUnitFlagCheckboxClicked);

	connect(ui->ingoreCountingGoalsFlagCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onIngoreCountingGoalsFlagCheckboxClicked);

	connect(ui->noArrivalMusicFlagCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoArrivalMusicFlagCheckboxClicked);

	connect(ui->noArrivalMessageFlagCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoArrivalMessageFlagCheckboxClicked);

	connect(ui->noFirstWaveMessageCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoFirstWaveMessageCheckboxClicked);

	connect(ui->noDynamicGoalsCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoDynamicGoalsCheckboxClicked);
	
	connect(ui->noArrivalWarpCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoArrivalWarpCheckboxClicked);

	connect(ui->noSpeedAdjustmentIfDockedArrivalCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoSpeedAdjustmentIfDockedArrivalCheckboxClicked);

	connect(ui->supercapWarpPhysicsArrivalCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onSupercapWarpPhysicsArrivalCheckboxClicked);

	connect(ui->noDepartureWarpCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoDepartureWarpCheckboxClicked);

	connect(ui->noSpeedAdjustmentIfDockedDepartureCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onNoSpeedAdjustmentIfDockedDepartureCheckboxClicked);
	
	connect(ui->supercapWarpPhysicsDepartureCheckbox, 
		&QCheckBox::clicked, 
		this, 
		&WingEditorDialog::onSupercapWarpPhysicsDepartureCheckboxClicked);


	connect(ui->arrivalTypeComboBox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onArrivalTypeComboBoxChanged);

	connect(ui->arrivalTargetComboBox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onArrivalTargetComboBoxChanged);

	connect(ui->departureTypeComboBox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onDepartureTypeComboBoxChanged);
		
	connect(ui->formationComboBox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onFormationComboBoxChanged);
		
	connect(ui->warpArrivalTypeCombobox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onWarpArrivalTypeComboboxChanged);
		
	connect(ui->warpDepartureTypeCombobox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onWarpDepartureTypeComboboxChanged);
		
	connect(ui->hotkeyComboBox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onHotkeyComboBoxChanged);

	connect(ui->selectWingCombobox,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&WingEditorDialog::onSelectWingComboboxChanged);


	// Set up wing list
	// TODO! This needs a signal to be updated whenever a wing is created or destroyed, 
	// AND ANOTHER SIGNAL to the current wing if there is a change in selection
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

void WingEditorDialog::loadWing() 
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
		
		ui->noArrivalWarpCheckbox->setChecked(false);
		ui->noArrivalWarpCheckbox->setEnabled(false);

		ui->noDepartureWarpCheckbox->setChecked(false);
		ui->noDepartureWarpCheckbox->setEnabled(false);

		ui->sameArrivalWarpWhenDockedCheckbox->setChecked(false);
		ui->sameArrivalWarpWhenDockedCheckbox->setEnabled(false);

		ui->sameDepartureWarpWhenDockedCheckbox->setChecked(false);
		ui->sameDepartureWarpWhenDockedCheckbox->setEnabled(false);
		

		
		ui->arrivalTargetComboBox->clear();
		ui->arrivalTargetComboBox->enable(false);		

		// TODO, this index should only be there when no wing is selected, for both arrival and departure
		ui->arrivalTypeComboBox->setCurrentIndex(0);
		ui->arrivalTypeComboBox->setEnabled(false);

		ui->initialArrivalDelaySpinBox->clear();
		ui->initialArrivalDelaySpinBox->setEnabled(false);

		ui->preDepartureDelaySpinBox->clear();
		ui->preDepartureDelaySpinBox->setEnabled(false);

		ui->minWaveDelayLineEdit->clear();
		ui->minWaveDelayLineEdit->setEnabled(false);

		ui->maxWaveDelayLineEdit->clear();
		ui->maxWaveDelayLineEdit->setEnabled(false);

	// Display all info based on the wing selected in the model
	} else {

		ui->wingNameLineEdit->setEnabled(true);
		ui->wingNameLineEdit->setText(wingName.c_str());

		ui->totalWavesSpinBox->setEnabled(true);
		ui->totalWavesSpinBox->setValue(_model->getNumberOfWaves());

		ui->hotkeyComboBox->setEnabled(true);
		ui->hotkeyComboBox->setCurrentIndex(_model->getHotkey() + 1); // Combo box thinks that "none" is index 0, so adjust

		auto leaderList = _model->getLeaderList();
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
		ui->ingoreCountingGoalsFlagCheckbox->setChecked(_model->getCountingGoalsFlag());

		ui->noArrivalMusicFlagCheckbox->setEnabled(true);
		ui->noArrivalMusicFlagCheckbox->setChecked(_model->getArrivalMusicFlag());

		ui->noArrivalMessageFlagCheckbox->setEnabled(true);
		ui->noArrivalMessageFlagCheckbox->setChecked(_model->getArrivalMessageFlag());

		ui->noFirstWaveMessageCheckbox->setEnabled(true);
		ui->noFirstWaveMessageCheckbox->setChecked(_model->getFirstWaveMessageFlag());

		ui->noDynamicGoalsCheckbox->setEnabled(true);
		ui->noDynamicGoalsCheckbox->setChecked(_model->getDynamicGoalsFlag());

		// warp parameters that don't have ui elements yet.
		
		ui->noArrivalWarpCheckbox->setEnabled(true);
		ui->noArrivalWarpCheckbox->setChecked(_model->getNoArrivalWarpFlag());

		ui->noDepartureWarpCheckbox->setEnabled(true);
		ui->noDepartureWarpCheckbox->setChecked(_model->getNoDepartureWarpFlag());

		ui->noSpeedAdjustmentIfDockedArrivalCheckbox->setEnabled(true);
		ui->noSpeedAdjustmentIfDockedArrivalCheckbox->setChecked(_model->getSameArrivalWarpWhenDockedFlag());

		ui->noSpeedAdjustmentIfDockedDepartureCheckbox->setEnabled(true);
		ui->noSpeedAdjustmentIfDockedDepartureCheckbox->setChecked(_model->getSameDepartureWarpWhenDockedFlag());
		

		auto arrivalTargetList = _model->getArrivalTargetList();

		// this does not seem to exist yet either.

		ui->arrivalTargetComboBox->enable(true);		
		ui->arrivalTargetComboBox->clear();

		for (const auto& string : arrivalTargetList.second) {
			ui->arrivalTargetComboBox.addItem(string.c_str());
		}

		ui->arrivalTargetcomboBox.setCurrentIndex(arrivalTargetList.first);

		auto departureList = getDepartureTargetList();
		/*
		ui->arrivalTypeComboBox->setEnabled(true);
		ui->arrivalTypeComboBox->setCurrentIndex(_model->getArrivalType() + 1);

		ui->departureTypeComboBox->setEnabled(true);
		// where's our getDepartureType function?
		//ui->departureTypeComboBox->setCurrentIndex(_model->getDepatureType() + 1);

		// not yet in the dialog?  
		auto departureList = _model->getDepartureTargetList();

		ui->departureTargetComboBox->setEnabled(true);
		ui->departureTargetComboBox->clear();

		for (const auto& string : departureList.second) {
			ui->arrivalTargetComboBox.addItem(string.c_str());
		}

		ui->departureTargetComboBox->setCurrentIndex(departureList.second);
		*/

		ui->initialArrivalDelaySpinBox->setEnabled(true);
		ui->initialArrivalDelaySpinBox->setValue(_model->getArrivalDistance());

		ui->preDepartureDelaySpinBox->setEnabled(true);
		ui->preDepartureDelaySpinBox->setValue(_model->getPredepartureDelay());

		ui->minWaveDelayLineEdit->setEnabled(true);
		ui->minWaveDelayLineEdit->setText(std::to_string(_model->getMinWaveDelay()).c_str());

		ui->maxWaveDelayLineEdit->setEnabled(true);
		ui->maxWaveDelayLineEdit->setText(std::to_string(_model->getMaxWaveDelay()).c_str());
		/*
		SCP_vector<SCP_string> getCurrentSelectableWings();

		// don't remember using this....
		int getInitialDelay();
		*/
	
	}

} 


void WingEditorDialog::onToggleGeneralOptionsButtonPressed()
{	
	static bool TODOwarn1 = false;
	if (!TODOwarn1) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn1 = true;
	}
}

void WingEditorDialog::onToggleArrivalDepartureButtonPressed()
{	
	static bool TODOwarn2 = false;
	if (!TODOwarn2) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn2 = true;
	}
}

void WingEditorDialog::onToggleWingFlagsButtonPressed()
{	
	static bool TODOwarn3 = false;
	if (!TODOwarn3) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn3 = true;
	}
}

void WingEditorDialog::onPreviousWingButtonPressed()
{	
	static bool TODOwarn4 = false;
	if (!TODOwarn4) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn4 = true;
	}
}

void WingEditorDialog::onNextWingButtonPressed()
{	
	static bool TODOwarn5 = false;
	if (!TODOwarn5) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn5 = true;
	}
}

void WingEditorDialog::onInitialOrdersButtonPressed()
{	
	static bool TODOwarn6 = false;
	if (!TODOwarn6) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn6 = true;
	}
}

void WingEditorDialog::onDeleteWingButtonPressed()
{	
	static bool TODOwarn7 = false;
	if (!TODOwarn7) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn7 = true;
	}
}

void WingEditorDialog::onDisbandWingButtonPressed()
{	
	static bool TODOwarn8 = false;
	if (!TODOwarn8) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn8 = true;
	}
}

void WingEditorDialog::onWarpArrivalStopSoundBrowseButtonPressed()
{	
	static bool TODOwarn9 = false;
	if (!TODOwarn9) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn9 = true;
	}
}

void WingEditorDialog::onWarpArrivalStopSoundPlayButtonPressed()
{	
	static bool TODOwarn21 = false;
	if (!TODOwarn21) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn21 = true;
	}
}

void WingEditorDialog::onWarpArrivalStartSoundBrowseButtonPressed()
{	
	static bool TODOwarn22 = false;
	if (!TODOwarn22) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn22 = true;
	}
}

void WingEditorDialog::onWarpArrivalStartSoundPlayButtonPressed()
{	
	static bool TODOwarn23 = false;
	if (!TODOwarn23) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn23 = true;
	}
}

void WingEditorDialog::onWarpArrivalAnimationBrowseButtonPressed()
{	
	static bool TODOwarn24 = false;
	if (!TODOwarn24) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn24 = true;
	}
}

void WingEditorDialog::onWarpDepartureStartSoundBrowseButtonPressed()
{	
	static bool TODOwarn25 = false;
	if (!TODOwarn25) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn25 = true;
	}
}

void WingEditorDialog::onWarpDepartureStartSoundPlayButtonPressed()
{	
	static bool TODOwarn26 = false;
	if (!TODOwarn26) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn26 = true;
	}
}

void WingEditorDialog::onWarpDepartureStopSoundBrowseButtonPressed()
{	
	static bool TODOwarn27 = false;
	if (!TODOwarn27) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn27 = true;
	}
}

void WingEditorDialog::onWarpDepartureStopSoundPlayButtonPressed()
{	
	static bool TODOwarn28 = false;
	if (!TODOwarn28) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn28 = true;
	}
}

void WingEditorDialog::onWarpDepartureAnimationBrowseButtonPressed()
{	
	static bool TODOwarn29 = false;
	if (!TODOwarn29) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn29 = true;
	}
}

void WingEditorDialog::onBrowseSquadLogosButtonPressed()
{	
	static bool TODOwarn30 = false;
	if (!TODOwarn30) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn30 = true;
	}
}


void WingEditorDialog::onReinforcementUnitFlagCheckboxClicked()
{	
	static bool TODOwarn31 = false;
	if (!TODOwarn31) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn31 = true;
	}
}

void WingEditorDialog::onIngoreCountingGoalsFlagCheckboxClicked()
{	
	static bool TODOwarn32 = false;
	if (!TODOwarn32) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn32 = true;
	}
}

void WingEditorDialog::onNoArrivalMusicFlagCheckboxClicked()
{	
	static bool TODOwarn33 = false;
	if (!TODOwarn33) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn33 = true;
	}
}

void WingEditorDialog::onNoArrivalMessageFlagCheckboxClicked()
{	
	static bool TODOwarn34 = false;
	if (!TODOwarn34) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn34 = true;
	}
}

void WingEditorDialog::onNoFirstWaveMessageCheckboxClicked()
{	
	static bool TODOwarn35 = false;
	if (!TODOwarn35) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn35 = true;
	}
}

void WingEditorDialog::onNoDynamicGoalsCheckboxClicked()
{	
	static bool TODOwarn36 = false;
	if (!TODOwarn36) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn36 = true;
	}
}


void WingEditorDialog::onArrivalDistanceSpinboxUpdated()
{	
	static bool TODOwarn37 = false;
	if (!TODOwarn37) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn37 = true;
	}
}

void WingEditorDialog::onMaxWaveDelaySpinBoxUpdated()
{	
	static bool TODOwarn38 = false;
	if (!TODOwarn38) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn38 = true;
	}
}

void WingEditorDialog::onMinWaveDelaySpinBoxUpdated()
{	
	static bool TODOwarn39 = false;
	if (!TODOwarn39) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn39 = true;
	}
}

void WingEditorDialog::onInitialArrivalDelaySpinBoxUpdated()
{	
	static bool TODOwarn40 = false;
	if (!TODOwarn40) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn40 = true;
	}
}

void WingEditorDialog::onPreDepartureDelaySpinBoxUpdated()
{	
	static bool TODOwarn41 = false;
	if (!TODOwarn41) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn41 = true;
	}
}

void WingEditorDialog::onWarpArrivalEngageTimeSpinBoxUpdated()
{	
	static bool TODOwarn42 = false;
	if (!TODOwarn42) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn42 = true;
	}
}

void WingEditorDialog::onWarpArrivalShipSpeedSpinboxUpdated()
{	
	static bool TODOwarn43 = false;
	if (!TODOwarn43) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn43 = true;
	}
}

void WingEditorDialog::onShipArrivalWarpingTimeSpinboxUpdated()
{	
	static bool TODOwarn44 = false;
	if (!TODOwarn44) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn44 = true;
	}
}

void WingEditorDialog::onWarpArrivalRadiusSpinboxUpdated()
{	
	static bool TODOwarn45 = false;
	if (!TODOwarn45) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn45 = true;
	}
}

void WingEditorDialog::onWarpDepartureEngageTimeSpinBoxUpdated()
{	
	static bool TODOwarn46 = false;
	if (!TODOwarn46) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn46 = true;
	}
}

void WingEditorDialog::onShipDepartureWarpingTimeSpinboxUpdated()
{	
	static bool TODOwarn47 = false;
	if (!TODOwarn47) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn47 = true;
	}
}

void WingEditorDialog::onWarpDepartureRadiusSpinboxUpdated()
{	
	static bool TODOwarn48 = false;
	if (!TODOwarn48) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn48 = true;
	}
}

void WingEditorDialog::onTotalWavesSpinBoxUpdated()
{	
	static bool TODOwarn49 = false;
	if (!TODOwarn49) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn49 = true;
	}
}

void WingEditorDialog::onWaveThresholdSpinBoxUpdated()
{	
	static bool TODOwarn50 = false;
	if (!TODOwarn50) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn50 = true;
	}
}


void WingEditorDialog::onReinforcementUnitFlagCheckboxClicked()
{	
	static bool TODOwarn51 = false;
	if (!TODOwarn51) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn51 = true;
	}
}

void WingEditorDialog::onIngoreCountingGoalsFlagCheckboxClicked()
{	
	static bool TODOwarn52 = false;
	if (!TODOwarn52) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn52 = true;
	}
}

void WingEditorDialog::onNoArrivalMusicFlagCheckboxClicked()
{	
	static bool TODOwarn53 = false;
	if (!TODOwarn53) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn53 = true;
	}
}

void WingEditorDialog::onNoArrivalMessageFlagCheckboxClicked()
{	
	static bool TODOwarn54 = false;
	if (!TODOwarn54) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn54 = true;
	}
}

void WingEditorDialog::onNoFirstWaveMessageCheckboxClicked()
{	
	static bool TODOwarn55 = false;
	if (!TODOwarn55) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn55 = true;
	}
}

void WingEditorDialog::onNoDynamicGoalsCheckboxClicked()
{	
	static bool TODOwarn56 = false;
	if (!TODOwarn56) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn56 = true;
	}
}

void WingEditorDialog::onNoArrivalWarpCheckboxClicked()
{	
	static bool TODOwarn57 = false;
	if (!TODOwarn57) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn57 = true;
	}
}

void WingEditorDialog::onNoSpeedAdjustmentIfDockedArrivalCheckboxClicked()
{	
	static bool TODOwarn58 = false;
	if (!TODOwarn58) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn58 = true;
	}
}

void WingEditorDialog::onSupercapWarpPhysicsArrivalCheckboxClicked()
{	
	static bool TODOwarn59 = false;
	if (!TODOwarn59) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn59 = true;
	}
}

void WingEditorDialog::onNoDepartureWarpCheckboxClicked()
{	
	static bool TODOwarn60 = false;
	if (!TODOwarn60) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn60 = true;
	}
}

void WingEditorDialog::onNoSpeedAdjustmentIfDockedDepartureCheckboxClicked()
{	
	static bool TODOwarn61 = false;
	if (!TODOwarn61) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn61 = true;
	}
}

void WingEditorDialog::onSupercapWarpPhysicsDepartureCheckboxClicked()
{	
	static bool TODOwarn62 = false;
	if (!TODOwarn62) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn62 = true;
	}
}


void WingEditorDialog::onArrivalTypeComboBoxChanged()
{	
	static bool TODOwarn63 = false;
	if (!TODOwarn63) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn63 = true;
	}
}

void WingEditorDialog::onArrivalTargetComboBoxChanged()
{	
	static bool TODOwarn64 = false;
	if (!TODOwarn64) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn64 = true;
	}
}

void WingEditorDialog::onDepartureTypeComboBoxChanged()
{	
	static bool TODOwarn65 = false;
	if (!TODOwarn65) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn65 = true;
	}
}

void WingEditorDialog::onFormationComboBoxChanged()
{	
	static bool TODOwarn66 = false;
	if (!TODOwarn66) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn66 = true;
	}
}

void WingEditorDialog::onWarpArrivalTypeComboboxChanged()
{	
	static bool TODOwarn67 = false;
	if (!TODOwarn67) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn67 = true;
	}
}

void WingEditorDialog::onWarpDepartureTypeComboboxChanged()
{	
	static bool TODOwarn68 = false;
	if (!TODOwarn68) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn68 = true;
	}
}

void WingEditorDialog::onHotkeyComboBoxChanged()
{	
	static bool TODOwarn69 = false;
	if (!TODOwarn69) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn69 = true;
	}
}

void WingEditorDialog::onSelectWingComboboxChanged()
{	
	static bool TODOwarn70 = false;
	if (!TODOwarn70) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn70 = true;
	}
}


void WingEditorDialog::onWarpArrivalStopSoundLineEditChanged()
{	
	static bool TODOwarn71 = false;
	if (!TODOwarn71) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn71 = true;
	}
}

void WingEditorDialog::onWarpArrivalStartSoundLineEditChanged()
{	
	static bool TODOwarn72 = false;
	if (!TODOwarn72) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn72 = true;
	}
}

void WingEditorDialog::onWarpArrivalAnimationLineEditChanged()
{	
	static bool TODOwarn73 = false;
	if (!TODOwarn73) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn73 = true;
	}
}

void WingEditorDialog::onWarpDepartureStartSoundLineEditChanged()
{	
	static bool TODOwarn74 = false;
	if (!TODOwarn74) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn74 = true;
	}
}

void WingEditorDialog::onWarpDepartureStopSoundLineEditChanged()
{	
	static bool TODOwarn75 = false;
	if (!TODOwarn75) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn75 = true;
	}
}

void WingEditorDialog::onWarpDepartureAnimationLineEdit()
{	
	static bool TODOwarn76 = false;
	if (!TODOwarn76) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn76 = true;
	}
}

void WingEditorDialog::onWingNameLineEditChanged()
{	
	static bool TODOwarn77 = false;
	if (!TODOwarn77) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn77 = true;
	}
}

void WingEditorDialog::onSquadLogoLineEditChanged()
{	
	static bool TODOwarn78 = false;
	if (!TODOwarn78) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn78 = true;
	}
}


void WingEditorDialog::onWarpArrivalDecelerationExponenetDoubleSpinboxUpdated()
{	
	static bool TODOwarn79 = false;
	if (!TODOwarn79) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn79 = true;
	}
}

void WingEditorDialog::onWarpDepartureDecelerationExponenetDoubleSpinboxUpdated()
{	
	static bool TODOwarn80 = false;
	if (!TODOwarn80) {
		QMessageBox warnbox;
		SCP_string message = "This control has not yet been set up.";
        warnbox.setText(message.c_str());
        warnbox.setStandardButtons(QMessageBox::Ok);
        warnbox.exec();

		TODOwarn80 = true;
	}
}



WingEditorDialog::~WingEditorDialog() {}; //NOLINT

} // dialogs
} // fred
} // fso