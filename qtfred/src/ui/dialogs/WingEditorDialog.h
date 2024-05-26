#pragma once

#include <QtWidgets/QDialog>
#include <mission/dialogs/WingEditorDialogModel.h>
#include <ui/FredView.h>
#include <ui/widgets/sexp_tree.h>


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
	void closeEvent(QCloseEvent*) override;

  private slots:

  private:
	std::unique_ptr<Ui::WingEditorDialog> ui;
	std::unique_ptr<WingEditorDialogModel> _model;
	EditorViewport* _viewport;

	void update();

};

} // namespace dialogs
} // namespace fred
} // namespace fso

