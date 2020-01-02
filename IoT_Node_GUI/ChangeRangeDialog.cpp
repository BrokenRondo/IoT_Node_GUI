#include "ChangeRangeDialog.h"
ChangeRangeDialog::ChangeRangeDialog(QWidget* parent)
{
	ui.setupUi(this);
	QDialog dialog(this);
	dialog.show();

}
int ChangeRangeDialog::getInput()
{
	return ui.lineEdit->text().toInt();
}

