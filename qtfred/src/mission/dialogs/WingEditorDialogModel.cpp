#include "WingEditorDialogModel.h"
#include <QMessageBox>
#include "FredApplication.h"

namespace fso {
namespace fred {
namespace dialogs {


WingEditorDialogModel::WingEditorDialogModel(QObject* parent, EditorViewport* viewport) 
		: AbstractDialogModel(parent, viewport)
{
	populateCurrentSelection();
}

void WingEditorDialogModel::populateCurrentSelection()
{
	object* objp = GET_FIRST(&obj_used_list);

	SCP_string candidateName;
	int lastIndex = -1;

	while (objp != END_OF_LIST(&obj_used_list)) {

		// do we have a marked ship?
		if ((objp->type == OBJ_SHIP || objp->type == OBJ_START) && objp->flags[Object::Object_Flags::Marked]) {

			// do we have a valid instance from that marked ship?
			if (objp->instance > -1 && objp->instance < MAX_SHIPS){

				// is that ship in a wing?
				if (Ships[objp->instance].wingnum > -1 && Ships[objp->instance].wingnum < MAX_WINGS){

					// First wing ship?
					if (lastIndex == -1){
						lastIndex = Ships[objp->instance].wingnum;

					// did this match the previous wing we found?
					} else if (lastIndex != Ships[objp->instance].wingnum) {
						// remove the value from last index because we didn't find a wing we could use.
						lastIndex = -1;
						break;
					}
				}
			}
		}
	}

	if (lastIndex < 0 || lastIndex >= MAX_WINGS){
		_currentWingIndex = -1;
		_currentWingName = "";
	} else {
		_currentWingIndex = lastIndex;
		_currentWingName = Wings[lastIndex].name;
	}
}


} // dialogs
} // fred
} // fso