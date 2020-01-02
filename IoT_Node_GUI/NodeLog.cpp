#include "NodeLog.h"
#include "ui_NodeLog.h"
#include <QLabel>

NodeLog::NodeLog(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::NodeLog();
	ui->setupUi(this);
}

NodeLog::~NodeLog()
{
	delete ui;
}
NodeLog::NodeLog(int index)
{
	ui = new Ui::NodeLog();
	ui->setupUi(this);
	ui->label->setText(tr("Node Log No.%1").arg(index));
	ui->plainTextEdit->document()->setMaximumBlockCount(2000);
}
void NodeLog::addLog(QString str)
{
	ui->plainTextEdit->appendPlainText(str);
}
