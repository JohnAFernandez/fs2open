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

/*	void onSwitchViewButtonPressed();
	void onShipListEdited();
	void onWeaponListEdited();
	void onExtraShipSpinboxUpdated();
	void onExtraWeaponSpinboxUpdated();
	void onExtraShipComboboxUpdated();
	void onExtraWeaponComboboxUpdated();
	void onPlayerDelayDoubleSpinBoxUpdated();
	void onCurrentTeamSpinboxUpdated();
	void onCopyLoadoutToOtherTeamsButtonPressed();
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
