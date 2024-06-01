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


WingEditorDialog::~WingEditorDialog() {}; //NOLINT

} // dialogs
} // fred
} // fso