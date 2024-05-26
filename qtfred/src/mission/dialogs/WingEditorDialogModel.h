#include "AbstractDialogModel.h"
#include "globalincs/pstypes.h"

namespace fso {
namespace fred {
namespace dialogs {

class WingEditorDialogModel : public AbstractDialogModel {
public:
	WingEditorDialogModel(QObject* parent, EditorViewport* viewport);
	
	bool apply() { return true; }
	void reject() {}

private:

	
};


}
}
}


