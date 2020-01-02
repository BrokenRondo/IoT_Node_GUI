#include "IoT_Node_GUI.h"
#include<QPainter>
#include <QVector>
#include <QString>
#include <QTimer>
#include "myline.h"
#include "actThread.h"
#include "IoT_Network_Scene.h"
#include "ChangeRangeDialog.h"
#include <queue>
extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
extern std::queue<QTimer*>GlobalTimerVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
extern unsigned int node_index;
extern int fieldSize;
extern int rangeSize;
extern time_t start;
extern time_t end;
extern unsigned int ConsensusGap;

IoT_Node_GUI::IoT_Node_GUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	IoT_Network_Scene* scene = new IoT_Network_Scene();
	scene->setSceneRect(-250, -250, 500, 500);
	this->Scene = scene;
	MyItem *myitem = new MyItem(node_index++,(unsigned int)12);
	myitem->setPos(0, 0);
	myitem->set_r(10);
	myitem->setType(1);
	scene->addItem(myitem->range);
	myitem->updatePos(0, 0);
	GlobalItemVector.push_back(myitem);
	scene->addItem(myitem);
	scene->add_an_item(myitem);
	scene->addItem(myitem->Text);
	
	ui.graphicsView->setScene(scene);
	ui.graphicsView->resize(1000, 800);
	ui.graphicsView->show();
	ui.horizontalSlider->setRange(0, 88);
	ui.horizontalSlider->setSliderPosition(65);
	ui.ConsensusSpeedSlider->setRange(0, 67);
	ui.ConsensusSpeedSlider->setSliderPosition(10);
	actThread* act_thread = new actThread();
	this->thread = act_thread;
	act_thread->setMainView(ui.graphicsView);
	connect(ui.startSim, SIGNAL(clicked()), this, SLOT(StartClicked()));
	connect(ui.Clear, SIGNAL(clicked()), this, SLOT(ClearClicked()));
	connect(ui.AddItem, SIGNAL(clicked()), this, SLOT(AddItemClicked()));
	connect(ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(ChangeSpeed(int)));
	connect(ui.ConsensusSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(ChangeConsensusSpeed(int)));
	connect(ui.StopSim, SIGNAL(clicked()), this, SLOT(StopClicked()));
	connect(ui.HideLog, SIGNAL(clicked()), this, SLOT(HideLog()));
	connect(ui.ShowLog, SIGNAL(clicked()), this, SLOT(ShowLog()));
	connect(ui.setRange, SIGNAL(clicked()), this, SLOT(setRange()));
	connect(ui.setField, SIGNAL(clicked()), this, SLOT(setField()));
	//QTimer *timer = new QTimer();
	//timer->setSingleShot(false);
	//timer->start(ConsensusGap);
}


void IoT_Node_GUI::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	//painter.drawLine(QPoint(0, 0), QPoint(100, 100));
}
void IoT_Node_GUI::setField()
{
	ChangeRangeDialog changeRangeDialog(this);
	changeRangeDialog.exec();
	int newRangeRadius = changeRangeDialog.getInput();
	fieldSize = newRangeRadius;
}
void IoT_Node_GUI::setRange()
{
	ChangeRangeDialog changeRangeDialog(this);
	changeRangeDialog.exec();
	int newRangeRadius = changeRangeDialog.getInput();
	rangeSize = newRangeRadius;
}

void IoT_Node_GUI::AddItemClicked()
{
	this->Scene->draw_an_item();
	if (started)
	{
		GlobalItemVector.back()->sender->start();
		GlobalItemVector.back()->consensus->Run();
		GlobalItemVector.back()->consensus->broadcast_->start();
	}
	this->update();
	ui.graphicsView->update();
}
void IoT_Node_GUI::StartClicked()
{

	//connect(act_thread, SIGNAL(toScene(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor)), this->Scene, SLOT(Scene_act_as_asked(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor)));
	//connect(act_thread, SIGNAL(toSceneUpdateText(unsigned int, QString)), this->Scene, SLOT(SlotUpdateLog(unsigned int, QString)));
	//connect(act_thread, SIGNAL(toSceneDeleteLines()), this->Scene, SLOT(SlotDeleteLineVector()));
	
	started = true;
	this->thread->start();
	for (int i=1;i<GlobalItemVector.size();i++)
	{
		GlobalItemVector[i]->sender->start();
		GlobalItemVector[i]->consensus->Run();
		GlobalItemVector[i]->consensus->broadcast_->start();
	}
}
void IoT_Node_GUI::StopClicked()
{
	started = false;
	for (int i = 0; i < GlobalItemVector.size(); i++)
	{
		GlobalItemVector[i]->sender->stop();
		GlobalItemVector[i]->consensus->stop();
		GlobalItemVector[i]->consensus->broadcast_->stop();
	}
	_sleep(1100);
	this->thread->setStop(true);
	this->Scene->SlotDeleteLineVector();
	GlobalLineVector.clear();
	//GlobalItemVector.clear();
	while (!GlobalTimerVector.empty())
	{
		GlobalTimerVector.pop();
	}
	
	//GlobalLogVector.clear();
}

void IoT_Node_GUI::ClearClicked()
{
	StopClicked();
	for (int i= GlobalItemVector.size() - 1;i>0;i--)
	{
		this->Scene->removeItem(GlobalItemVector[i]);
		this->Scene->removeItem(GlobalTextVector[i]);
		this->Scene->removeItem(GlobalItemVector[i]->Text);
		this->Scene->removeItem(GlobalItemVector[i]->range);
		GlobalItemVector[i]->nodelog->close();
		GlobalItemVector[i]->nodeBlockchainDatabase->close();
		GlobalItemVector[i]->sender->close();
		GlobalItemVector[i]->receiver->close();
		delete GlobalItemVector[i]->nodeBlockchainDatabase;
		delete GlobalItemVector[i]->nodelog;
		delete GlobalItemVector[i];
		GlobalItemVector.erase(GlobalItemVector.begin()+i);
	}
	for (int i = 0; i < GlobalLineVector.size(); i++)
	{
		this->Scene->removeItem(GlobalLineVector[i]);
		delete GlobalLineVector[i];
	}
	//for (int i=GlobalTimerVector.size()-1; i>=0;i--)
	//{
	//	delete GlobalTimerVector[i];
	//	GlobalTimerVector.remove(i);
	//}

	GlobalLogVector.clear();
	GlobalLineVector.clear();
	GlobalItemVector.clear();
	node_index = 0;
	this->Scene->clear();
	this->Scene->draw_an_item(QPointF(0, 0));
	qApp->closeAllWindows();

	QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}
void IoT_Node_GUI::ChangeSpeed(int speed)
{
	for (int i=0;i<GlobalItemVector.size();i++)
	{
		GlobalItemVector[i]->sender->setFrequency(100 - speed);
		GlobalItemVector[i]->sender->setSpeed(1000 - 10 * speed);
	}
}

void IoT_Node_GUI::ChangeConsensusSpeed(int speed)
{
	ConsensusGap = 8000 + speed*speed*speed;
}

void IoT_Node_GUI::HideLog()
{
	for (int i=0;i<GlobalTextVector.size();i++)
	{
		GlobalTextVector[i]->setVisible(false);
	}
}
void IoT_Node_GUI::ShowLog()
{
	for (int i = 0; i < GlobalTextVector.size(); i++)
	{
		GlobalTextVector[i]->setVisible(true);
	}
}
void IoT_Node_GUI::slotTimerUpdate()
{
	start = end;
	end = end + ConsensusGap;
}

