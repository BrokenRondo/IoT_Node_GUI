#pragma once
#ifndef C_R_DIALOG
#define C_R_DIALOG
#include "ui_ChangeRangeDialog.h"
class ChangeRangeDialog:public QDialog
{
	Q_OBJECT
public:
	ChangeRangeDialog(QWidget *parent);
	int getInput();
private:
	Ui::Dialog ui;
};

#endif
