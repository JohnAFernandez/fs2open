#include "WingEditorDialog.h"
#include <ui_WingEditorDialog.h>


namespace fso {
namespace fred {
namespace dialogs {


WingEditorDialog::WingEditorDialog(FredView* parent, EditorViewport* viewport) 
	: QDialog(parent), ui(new Ui::WingEditorDialog()), _model(new WingEditorDialogModel(this, viewport)), _viewport(viewport)
{
	this->setFocus();
	ui->setupUi(this);
	resize(QDialog::sizeHint()); // The best I can tell without some research, when a dialog doesn't use an underlying grid or layout, it needs to be resized this way before anything will show up 

	// apply and reject do not ... uh ... apply here because changes are in real time.




}

WingEditorDialog::~WingEditorDialog() {}; //NOLINT

} // dialogs
} // fred
} // fso