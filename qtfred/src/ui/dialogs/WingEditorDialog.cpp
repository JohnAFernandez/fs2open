#include "WingEditorDialog.h"
#include "ui_WingEditorDialog.h"

namespace fso {
namespace fred {
namespace dialogs {


WingEditorDialog::WingEditorDialog(FredView* parent, EditorViewport* viewport) : QDialog(parent), ui(new Ui::WingEditorDialog()), _model(new WingEditorDialogModel(this, viewport)), _viewport(viewport)
{

}

} // dialogs
} // fred
} // fso