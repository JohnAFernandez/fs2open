#include "LoadoutDialog.h"
#include "ui_LoadoutDialog.h"

#include <QtWidgets/QMenuBar>
#include <qlist.h>
#include <qtablewidget.h>

constexpr int TABLE_MODE = 0;
constexpr int VARIABLE_MODE = 1;

// header text
constexpr char* SHIPHEADER = "Ship";
constexpr char* WEAPONHEADER = "Weapon";
constexpr char* KEYHEADER = "In wings / Extra / Total";


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
		static_cast<void (QTableWidget::*)(QTableWidgetItem*)>(&QTableWidget::itemClicked),
		this,
		&LoadoutDialog::onShipListEdited);

	connect(ui->weaponVarList,
		static_cast<void (QTableWidget::*)(QTableWidgetItem*)>(&QTableWidget::itemClicked),
		this,
		&LoadoutDialog::onWeaponListEdited);

	// things that must be set for everything to work...
	_mode = TABLE_MODE;
	
	// rows will vary but columns need to be 2
	ui->shipVarList->setColumnCount(2);
	ui->weaponVarList->setColumnCount(2);
	
	ui->shipVarList->setHorizontalHeaderItem(0, new QTableWidgetItem(SHIPHEADER));

	ui->weaponVarList->setHorizontalHeaderItem(0, new QTableWidgetItem(WEAPONHEADER));

	ui->shipVarList->setHorizontalHeaderItem(1, new QTableWidgetItem(KEYHEADER));
	ui->weaponVarList->setHorizontalHeaderItem(1, new QTableWidgetItem(KEYHEADER));

	// need to completely rebuild the lists here.
	resetLists();
	updateUI();
}

LoadoutDialog::~LoadoutDialog(){} // NOLINT

void LoadoutDialog::onSwitchViewButtonPressed()
{
	if (_mode == TABLE_MODE) {
		ui->tableVarLabel->setText("Enable Via Variable View");
		ui->startingShipsLabel->setText("Ship-Enabling Variables");
		ui->startingWeaponsLabel->setText("Weapon-Enabling Variables");
		_mode = VARIABLE_MODE;
	}
	else {
		ui->tableVarLabel->setText("Table Entry View");
		ui->startingShipsLabel->setText("Starting Ships");
		ui->startingWeaponsLabel->setText("Starting Weapons");
		_mode = TABLE_MODE;
	}

	// model does not keep track of whether the UI is editing the table values or the vars
	// so, just reset the lists and then update the UI
	resetLists();
	updateUI();
}

void LoadoutDialog::onShipListEdited()
{
	sendEditedShips();
}

void LoadoutDialog::onWeaponListEdited()
{
	sendEditedWeapons();
}

void LoadoutDialog::onExtraShipSpinboxUpdated()
{
	sendEditedShips();
}

void LoadoutDialog::onExtraWeaponSpinboxUpdated()
{
	sendEditedWeapons();
}

void LoadoutDialog::onExtraShipComboboxUpdated()
{
	sendEditedShips();
}

void LoadoutDialog::onExtraWeaponComboboxUpdated()
{
	sendEditedWeapons();
}

void LoadoutDialog::onPlayerDelayDoubleSpinBoxUpdated()
{
	_model->setPlayerEntryDelay(static_cast<float>(ui->playerDelayDoubleSpinbox->value()));
}

void LoadoutDialog::onCurrentTeamSpinboxUpdated()
{
	_model->switchTeam(ui->currentTeamSpinbox->value());
}

void LoadoutDialog::onCopyLoadoutToOtherTeamsButtonPressed()
{
	_model->copyToOtherTeam();
}

void LoadoutDialog::sendEditedShips()
{
	SCP_vector<SCP_string> namesOut;
	bool enabled = false;

	for (auto& item : ui->shipVarList->selectedItems()) {
		namesOut.push_back(ui->shipVarList->itemAt(item->row(), 0)->text().toStdString());
		enabled = (item->checkState() == Qt::Checked); // TODO! Make sure that all items are changed to enabled or disabled before we get here
	}

	if (_mode == TABLE_MODE) {
		// why did I do it this way?  I don't know. Sorry. I wrote the model first.
		for (auto& nameOut : namesOut){
			_model->setShipInfo(nameOut, enabled, ui->extraShipSpinbox->value(), ui->extraShipsViaVarCombo->currentText().toStdString());
		}
	}
	else {
		_model->setShipEnablerVariables(namesOut, enabled, ui->extraShipSpinbox->value(), ui->extraShipsViaVarCombo->currentText().toStdString());
	}

	updateUI(); // Better to call it here, than over and over with a modelChanged
}

// to simplify things on our end, send everything about whatever is selected.
void LoadoutDialog::sendEditedWeapons()
{
	SCP_vector<SCP_string> namesOut;
	bool enabled = false;

	for (auto& item : ui->weaponVarList->selectedItems()) {
		namesOut.push_back(ui->weaponVarList->itemAt(item->row(), 0)->text().toStdString());
		enabled = (item->checkState() == Qt::Checked); // TODO! Make sure that all items are changed to enabled or disabled before we get here
	}

	if (_mode == TABLE_MODE) {
		// why did I do it this way?  Bad assumption, sorry. I wrote the model first.
		for (auto& nameOut : namesOut){
			_model->setWeaponInfo(nameOut, enabled, ui->extraShipSpinbox->value(), ui->extraShipsViaVarCombo->currentText().toStdString());
		}
	}
	else {
		_model->setWeaponEnablerVariables(namesOut, enabled, ui->extraShipSpinbox->value(), ui->extraShipsViaVarCombo->currentText().toStdString());
	}

	updateUI();
}

void LoadoutDialog::updateUI()
{
	SCP_vector<std::pair<SCP_string, bool>> newShipList;
	SCP_vector<std::pair<SCP_string, bool>> newWeaponList;

	// repopulate with the correct lists from the model.
	if (_mode == TABLE_MODE) {
		newShipList = _model->getShipList();
		newWeaponList = _model->getWeaponList();
	}
	else {
		newShipList = _model->getShipEnablerVariables();
		newWeaponList = _model->getWeaponEnablerVariables();
	}

	int currentRow = 0;

	// build the ship list...
	for (auto& newShip : newShipList) {
		// need to split the incoming string into the different parts.
		size_t divider = newShip.first.find_last_of(":");

		// Overwrite the old number text.
		ui->shipVarList->item(currentRow, 1)->setText(newShip.first.substr(divider + 2).c_str());

		// enable the check box, if necessary
		(newShip.second) ? ui->shipVarList->item(currentRow, 0)->setCheckState(Qt::Checked) : ui->shipVarList->item(currentRow, 0)->setCheckState(Qt::Unchecked);

		currentRow++;
	}

	currentRow = 0;

	for (auto& newWeapon : newWeaponList) {
		// need to split the incoming string into the different parts.
		size_t divider = newWeapon.first.find_last_of(":");

		// Overwrite the old number text.
		ui->weaponVarList->item(currentRow, 1)->setText(newWeapon.first.substr(divider + 2).c_str());

		// enable the check box, if necessary
		(newWeapon.second) ? ui->weaponVarList->item(currentRow, 0)->setCheckState(Qt::Checked) : ui->weaponVarList->item(currentRow, 0)->setCheckState(Qt::Unchecked);

		currentRow++;
	}

	SCP_vector<SCP_string> namesOut;
	// get all selected ship items to send to the model
	for (auto& item : ui->shipVarList->selectedItems()) {
		namesOut.push_back(ui->shipVarList->itemAt(item->row(), 0)->text().toStdString());
	}

	// request info for combo and extra ship box.
	if (_mode == TABLE_MODE) {
		ui->extraShipsViaVarCombo->setCurrentText(_model->getCountVarShips(namesOut).c_str()); // TODO in the future, loop through current choices.
		ui->extraShipSpinbox->setValue(_model->getExtraAllocatedShips(namesOut));
	}
	else {
		ui->extraShipsViaVarCombo->setCurrentText(_model->getCountVarShipEnabler(namesOut).c_str());
		ui->extraShipSpinbox->setValue(_model->getExtraAllocatedShipEnabler(namesOut));
	}

	namesOut.clear();

	for (auto& item : ui->weaponVarList->selectedItems()) {
		namesOut.push_back(ui->weaponVarList->itemAt(item->row(), 0)->text().toStdString());
	}

	if (_mode == TABLE_MODE) {
		ui->extraWeaponsViaVarCombo->setCurrentText(_model->getCountVarWeapons(namesOut).c_str()); // TODO in the future, loop through current choices.
		ui->extraWepSpinbox->setValue(_model->getExtraAllocatedWeapons(namesOut));
	}
	else {
		ui->extraWeaponsViaVarCombo->setCurrentText(_model->getCountVarWeaponEnabler(namesOut).c_str());
		ui->extraWepSpinbox->setValue(_model->getExtraAllocatedWeaponEnabler(namesOut));
	}

	ui->playerDelayDoubleSpinbox->setValue(_model->getPlayerEntryDelay());
	ui->currentTeamSpinbox->setValue(_model->getCurrentTeam() + 1);


	// reselect those that were previously selected 
//	for (auto& savedSelection : saveListShips) {
//		QList<QTableWidgetItem *> foundItems = ui->shipVarList->findItems(savedSelection.c_str(), Qt::MatchStartsWith);
//		for (auto& item : foundItems) {
//			ui->shipVarList->selectRow(item->row());
//		}
//	}

//	for (auto& savedSelection : saveListWeapons) {
//		QList<QTableWidgetItem *> foundItems = ui->shipVarList->findItems(savedSelection.c_str(), Qt::MatchStartsWith);
//		for (auto& item : foundItems) {
//			ui->weaponVarList->selectRow(item->row());
//		}
//	}

}

void LoadoutDialog::resetLists() {
	// clear the lists
	ui->shipVarList->clearContents();
	ui->weaponVarList->clearContents();

	SCP_vector<std::pair<SCP_string, bool>> newShipList;
	SCP_vector<std::pair<SCP_string, bool>> newWeaponList;

	// repopulate with the correct lists from the model.
	if (_mode == TABLE_MODE) {
		newShipList = _model->getShipList();
		newWeaponList = _model->getWeaponList();
	}
	else {
		newShipList = _model->getShipEnablerVariables();
		newWeaponList = _model->getWeaponEnablerVariables();
	}

	ui->shipVarList->setRowCount(static_cast<int>(newShipList.size()));
	ui->weaponVarList->setRowCount(static_cast<int>(newWeaponList.size()));

	int currentRow = 0;

	// build the ship list...
	for (auto& newShip : newShipList) {
		// need to split the incoming string into the different parts.
		size_t divider = newShip.first.find_last_of(":");

		// add text to the items
		QTableWidgetItem* nameItem = new QTableWidgetItem(newShip.first.substr(0, divider).c_str());
		QTableWidgetItem* countItem = new QTableWidgetItem(newShip.first.substr(divider + 2).c_str());

		// enable the check box, if necessary
		(newShip.second) ? nameItem->setCheckState(Qt::Checked) : nameItem->setCheckState(Qt::Unchecked);

		// overwrite the entry in the table.
		ui->shipVarList->setItem(currentRow, 0, nameItem);
		ui->shipVarList->setItem(currentRow, 1, countItem);

		currentRow++;
	}

	currentRow = 0;

	for (auto& newWeapon : newWeaponList) {
		// need to split the incoming string into the different parts.
		size_t divider = newWeapon.first.find_last_of(":");

		// add text to the items
		QTableWidgetItem* nameItem = new QTableWidgetItem(newWeapon.first.substr(0, divider).c_str());
		QTableWidgetItem* countItem = new QTableWidgetItem(newWeapon.first.substr(divider + 2).c_str());

		// enable the check box, if necessary
		(newWeapon.second) ? nameItem->setCheckState(Qt::Checked) : nameItem->setCheckState(Qt::Unchecked);

		// overwrite the entry in the table.
		ui->weaponVarList->setItem(currentRow, 0, nameItem);
		ui->weaponVarList->setItem(currentRow, 1, countItem);

		currentRow++;
	}
}

}
}
}
