#include "LoadoutDialog.h"
#include "ui_LoadoutDialog.h"

#include <QtWidgets/QMenuBar>

namespace fso {
namespace fred {
namespace dialogs {

LoadoutDialog::LoadoutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeamLoadoutDialog)
{
	this->setFocus();
    ui->setupUi(this);

	connect(_model.get(), &AbstractDialogModel::modelChanged, this, &CommandBriefingDialog::updateUI);
	connect(this, &QDialog::accepted, _model.get(), &CommandBriefingDialogModel::apply);
	connect(this, &QDialog::rejected, _model.get(), &CommandBriefingDialogModel::reject);

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



TeamLoadoutDialog::~TeamLoadoutDialog()
{
    delete ui;
}

}
}
}
