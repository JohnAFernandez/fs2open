#pragma once

#include <QtWidgets/QDialog>
#include <mission/dialogs/WingEditorDialogModel.h>
#include <ui/FredView.h>
#include <ui/widgets/sexp_tree.h>
#include <ui_WingEditorDialog.h>


namespace fso {
namespace fred {
namespace dialogs {

namespace Ui {
class WingEditorDialog;
}

/**
* @brief QTFred's Wing Editor
*/
class WingEditorDialog : public QDialog, public SexpTreeEditorInterface {

	Q_OBJECT

  public:
	/**
	 * @brief Constructor
	 * @param parent The main fred window. Needed for triggering window updates.
	 * @param viewport The viewport this dialog is attacted to.
	 */
	explicit WingEditorDialog(FredView* parent, EditorViewport* viewport);
	~WingEditorDialog() override;

	/**
	 * @brief Allows subdialogs to get the ship the editor is currently working on.
	 * @return Returns the index in Wings array if working on one or -1 no wing is selected.
	 */
	int getCurrentWing() const;

  protected:

  private slots:
void onToggleGeneralOptionsButtonPressed();
void onToggleArrivalDepartureButtonPressed();
void onToggleWingFlagsButtonPressed();
void onPreviousWingButtonPressed();
void onNextWingButtonPressed();
void onInitialOrdersButtonPressed();
void onDeleteWingButtonPressed();
void onDisbandWingButtonPressed();
void onWarpArrivalStopSoundBrowseButtonPressed();
void onWarpArrivalStopSoundPlayButtonPressed();
void onWarpArrivalStartSoundBrowseButtonPressed();
void onWarpArrivalStartSoundPlayButtonPressed();
void onWarpArrivalAnimationBrowseButtonPressed();
void onWarpDepartureStartSoundBrowseButtonPressed();
void onWarpDepartureStartSoundPlayButtonPressed();
void onWarpDepartureStopSoundBrowseButtonPressed();
void onWarpDepartureStopSoundPlayButtonPressed();
void onWarpDepartureAnimationBrowseButtonPressed();
void onBrowseSquadLogosButtonPressed();

void onReinforcementUnitFlagCheckboxClicked();
void onIngoreCountingGoalsFlagCheckboxClicked();
void onNoArrivalMusicFlagCheckboxClicked();
void onNoArrivalMessageFlagCheckboxClicked();
void onNoFirstWaveMessageCheckboxClicked();
void onNoDynamicGoalsCheckboxClicked();
void onNoArrivalWarpCheckboxClicked();
void onNoSpeedAdjustmentIfDockedArrivalCheckboxClicked();
void onSupercapWarpPhysicsArrivalCheckboxClicked();
void onNoDepartureWarpCheckboxClicked();
void onNoSpeedAdjustmentIfDockedDepartureCheckboxClicked();
void onSupercapWarpPhysicsDepartureCheckboxClicked();


void onArrivalDistanceSpinboxUpdated();
void onMaxWaveDelaySpinBoxUpdated();
void onMinWaveDelaySpinBoxUpdated();
void onInitialArrivalDelaySpinBoxUpdated();
void onPreDepartureDelaySpinBoxUpdated();
void onWarpArrivalEngageTimeSpinBoxUpdated();
void onWarpArrivalShipSpeedSpinboxUpdated();
void onShipArrivalWarpingTimeSpinboxUpdated();
void onWarpArrivalRadiusSpinboxUpdated();
void onWarpDepartureEngageTimeSpinBoxUpdated();
void onShipDepartureWarpingTimeSpinboxUpdated();
void onWarpDepartureRadiusSpinboxUpdated();
void onTotalWavesSpinBoxUpdated();
void onWaveThresholdSpinBoxUpdated();


void onArrivalTypeComboBoxChanged();
void onArrivalTargetComboBoxChanged();
void onDepartureTypeComboBoxChanged();
void onFormationComboBoxChanged();
void onWarpArrivalTypeComboboxChanged();
void onWarpDepartureTypeComboboxChanged();
void onHotkeyComboBoxChanged();
void onSelectWingComboboxChanged();

void onWarpArrivalStopSoundLineEditChanged();
void onWarpArrivalStartSoundLineEditChanged();
void onWarpArrivalAnimationLineEditChanged();
void onWarpDepartureStartSoundLineEditChanged();
void onWarpDepartureStopSoundLineEditChanged();
void onWarpDepartureAnimationLineEdit();
void onWingNameLineEditChanged();
void onSquadLogoLineEditChanged();

void onWarpArrivalDecelerationExponenetDoubleSpinboxUpdated();

void onWarpDepartureDecelerationExponenetDoubleSpinboxUpdated();

  private:
	std::unique_ptr<Ui::WingEditorDialog> ui;
	std::unique_ptr<WingEditorDialogModel> _model;
	EditorViewport* _viewport;

	void loadWing();
	bool _updating = false;
	
	
	void adjustDialogSize();

	const int _closedHeight = 22;

	// Variables for tracking the size of the dialog and its groupBoxes so that they can be collapsed
	int _originalHeightDialog;
	
	int _originalHeightGeneral;
	int _originalYGeneral;
	bool _generalOpen = true;

	int _originalHeightFlags;
	int _originalYFlags;
	bool _flagsOpen = true;

	int _originalHeightTabs;
	int _originalYTabs;
	bool _tabsOpen = true;

	int _originalArrivalDepartureButtonLocation;
	int _originalArrivalDepartureLabelLocation;
};

} // namespace dialogs
} // namespace fred
} // namespace fso

