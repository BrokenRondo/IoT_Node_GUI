#pragma once

#include <QWidget>
namespace Ui { class NodeLog; };

class NodeLog : public QWidget
{
	Q_OBJECT

public:
	NodeLog(QWidget *parent = Q_NULLPTR);
	NodeLog(int index);
	~NodeLog();
	void addLog(QString);
public:
	Ui::NodeLog *ui;
	
private slots:
	
};
