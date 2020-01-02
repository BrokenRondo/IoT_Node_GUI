#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_IoT_Node_GUI.h"
#include "IoT_Network_Scene.h"
#include "actThread.h"


class IoT_Node_GUI : public QMainWindow
{
	Q_OBJECT

public:
	IoT_Node_GUI(QWidget *parent = Q_NULLPTR);

private:
	Ui::IoT_Node_GUIClass ui;
	IoT_Network_Scene * Scene;
	InteractiveView* View;
	actThread* thread = nullptr;
	bool started = false;
private slots:
	void AddItemClicked();
	void StartClicked();
	void ClearClicked();
	void StopClicked();
	void ChangeSpeed(int);
	void HideLog();
	void ShowLog();
	void setField();
	void setRange();
	void slotTimerUpdate();
	void ChangeConsensusSpeed(int speed);
protected:
	void paintEvent(QPaintEvent* event);
};
