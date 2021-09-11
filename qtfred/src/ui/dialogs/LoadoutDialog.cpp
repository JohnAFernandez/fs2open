#include "LoadoutDialog.h"
#include "ui_LoadoutDialog.h"

#include <QtWidgets/QMenuBar>
#include <qlist.h>
#include <qtablewidget.h>

constexpr int TABLE_MODE = 0;
constexpr int VARIABLE_MODE = 1;


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

	_mode = TABLE_MODE;
	ui->shipVarList->setColumnCount(2);
	ui->weaponVarList->setColumnCount(2);

	updateUI();
}

LoadoutDialog::~LoadoutDialog(){} // NOLINT

void LoadoutDialog::onSwitchViewButtonPressed()
{
	if (_mode = TABLE_MODE) {
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
	// so, just update the UI
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
		SCP_string workingCopy = item->text().toStdString();
		size_t location = workingCopy.find_last_of(":"); // the character the separates the name and numbers
		workingCopy.erase(workingCopy.begin() + location, workingCopy.end()-1);
		namesOut.push_back(workingCopy);
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
		SCP_string workingCopy = item->text().toStdString();
		size_t location = workingCopy.find_last_of(":"); // the character the separates the name and numbers
		workingCopy.erase(workingCopy.begin() + location, workingCopy.end()-1);
		namesOut.push_back(workingCopy);
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
	SCP_vector<SCP_string> saveListShips;
	SCP_vector<SCP_string> saveListWeapons;

	// save all currently selected Items
	for (auto& item : ui->shipVarList->selectedItems()) {
		SCP_string workingCopy = item->text().toStdString();
		size_t location = workingCopy.find_last_of(":"); // the character the separates the name and numbers
		workingCopy.erase(workingCopy.begin() + location, workingCopy.end()-1);
		saveListShips.push_back(workingCopy);
	}

	for (auto& item : ui->weaponVarList->selectedItems()) {
		SCP_string workingCopy = item->text().toStdString();
		size_t location = workingCopy.find_last_of(":"); // the character the separates the name and numbers
		workingCopy.erase(workingCopy.begin() + location, workingCopy.end()-1);
		saveListWeapons.push_back(workingCopy);
	}
	
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

	for (auto& newShip : newShipList) {
		QTableWidgetItem *listItem = new QTableWidgetItem(newShip.first.c_str());
		(newShip.second) ? listItem->setCheckState(Qt::Checked) : listItem->setCheckState(Qt::Unchecked);

		ui->shipVarList->setItem(currentRow, 0, listItem);
		currentRow++;
	}

	currentRow = 0;

	for (auto& newWeapon : newWeaponList) {
		QTableWidgetItem *listItem = new QTableWidgetItem(newWeapon.first.c_str());
		(newWeapon.second) ? listItem->setCheckState(Qt::Checked) : listItem->setCheckState(Qt::Unchecked);
		ui->weaponVarList->setItem(currentRow, 0, listItem);
	}

	// reselect those that were previously selected 
	for (auto& savedSelection : saveListShips) {
		QList<QTableWidgetItem *> foundItems = ui->shipVarList->findItems(savedSelection.c_str(), Qt::MatchStartsWith);
		for (auto& item : foundItems) {
			item->setSelected(true);
		}
	}

	for (auto& savedSelection : saveListWeapons) {
		QList<QTableWidgetItem *> foundItems = ui->shipVarList->findItems(savedSelection.c_str(), Qt::MatchStartsWith);
		for (auto& item : foundItems) {
			item->setSelected(true);
		}
	}

	// TODO! update random spinboxes and comboboxes. if the values differ, clear them out.
	// it looks like I'm going to need to write a few more lines in the model to do this.
}

}
}
}
