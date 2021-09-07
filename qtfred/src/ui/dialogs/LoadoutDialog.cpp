#include "LoadoutDialog.h"
#include "ui_LoadoutDialog.h"

#include <QtWidgets/QMenuBar>

namespace fso {
namespace fred {
namespace dialogs {

LoadoutDialog::LoadoutDialog(FredView* parent, EditorViewport* viewport) 
	: QDialog(parent), ui(new Ui::LoadoutDialog()), _model(new LoadoutDialogModel(this, viewport)),
		_viewport(viewport)
{
	this->setFocus();
    ui->setupUi(this);

	connect(_model.get(), &AbstractDialogModel::modelChanged, this, &LoadoutDialog::updateUI);
	connect(this, &QDialog::accepted, _model.get(), &LoadoutDialogModel::apply);
	connect(this, &QDialog::rejected, _model.get(), &LoadoutDialogModel::reject);

	connect(ui->currentTeamSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&LoadoutDialog::onCurrentTeamSpinboxUpdated);

	connect(ui->extraShipSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&LoadoutDialog::onExtraShipSpinboxUpdated);

	connect(ui->extraWepSpinbox,
		static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
		this,
		&LoadoutDialog::onExtraWeaponSpinboxUpdated);
	
	connect(ui->playerDelayDoubleSpinbox,
		static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
		this,
		&LoadoutDialog::onPlayerDelayDoubleSpinBoxUpdated);

	connect(ui->extraShipsViaVarCombo,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&LoadoutDialog::onExtraShipComboboxUpdated);

	connect(ui->extraWeaponsViaVarCombo,
		QOverload<int>::of(&QComboBox::currentIndexChanged),
		this,
		&LoadoutDialog::onExtraWeaponComboboxUpdated);

	connect(ui->copyLoadoutToOtherTeamsButton,
		&QPushButton::clicked,
		this,
		&LoadoutDialog::onCopyLoadoutToOtherTeamsButtonPressed);

	connect(ui->switchViewButton,
		&QPushButton::clicked,
		this,
		&LoadoutDialog::onSwitchViewButtonPressed);

	connect(ui->shipVarList,
		static_cast<void (QListWidget::*)(QListWidgetItem*)>(&QListWidget::itemClicked),
		this,
		&LoadoutDialog::onShipListEdited);

	connect(ui->weaponVarList,
		static_cast<void (QListWidget::*)(QListWidgetItem*)>(&QListWidget::itemClicked),
		this,
		&LoadoutDialog::onWeaponListEdited);


	updateUI();

/*

	void onSwitchViewButtonPressed();
	void onExtraShipComboboxUpdated();
	void onExtraWeaponComboboxUpdated();
	void onPlayerDelayDoubleSpinBoxUpdated();
	void onCurrentTeamSpinboxUpdated();
	void onExtraShipSpinboxUpdated();
	void onExtraWeaponSpinboxUpdated();
	void onCopyLoadoutToOtherTeamsButtonPressed();
	void onShipListEdited();
	void onWeaponListEdited();


	*/


    /*
     * Note: For the weapon and ship loadout listWidgest, will need to do something similar to the following during loading to
     * add each ship and weapon option as a checkbox
    QStringList  itemLabels= getLabels();

    QStringListIterator it(itemLabels);
    while (it.hasNext())
    {
          QListWidgetItem *listItem = new QListWidgetItem(it.next(),listWidget);
          listItem->setCheckState(Qt::Unchecked);
          ui->listWidget->addItem(listItem);
    }
    */
}



LoadoutDialog::~LoadoutDialog(){} // NOLINT

void LoadoutDialog::onSwitchViewButtonPressed()
{
	// this one is complicated, do it later, lol
}

void LoadoutDialog::onShipListEdited()
{

}

void LoadoutDialog::onWeaponListEdited()
{

}

void LoadoutDialog::onExtraShipSpinboxUpdated()
{

}

void LoadoutDialog::onExtraWeaponSpinboxUpdated()
{

}

void LoadoutDialog::onExtraShipComboboxUpdated()
{

}

void LoadoutDialog::onExtraWeaponComboboxUpdated()
{

}

void LoadoutDialog::onPlayerDelayDoubleSpinBoxUpdated()
{
	_model->setPlayerEntryDelay(static_cast<float>(ui->playerDelayDoubleSpinbox->value()));
}

void LoadoutDialog::onCurrentTeamSpinboxUpdated()
{
//	_model->switchTeam()
}

void LoadoutDialog::onCopyLoadoutToOtherTeamsButtonPressed()
{
	_model->copyToOtherTeam();
}

void LoadoutDialog::updateUI()
{

}

}
}
}
