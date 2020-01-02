#include "IoT_Network_Scene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QString>
extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
extern QQueue<QTimer*> GlobalTimerVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
extern unsigned int node_index;
void IoT_Network_Scene::Scene_act_as_asked(unsigned int msg_type, MyItem* item, MyLine*line, QPointF a, QPointF b, QPointF c, QPointF d, QColor color, unsigned int index)
{
	switch(msg_type)
	{
	case 1:
		draw_an_arrow(a,b,color,index);
	case 2:
		draw_an_arrow(a, b, color, index);
	case 3:
		
	default:
		break;
	}
}
void IoT_Network_Scene::add_an_item(QGraphicsItem* item)
{
	this->graphics_items.append(item);
}
void IoT_Network_Scene::add_a_line(QGraphicsItem* line)
{
	this->graphics_lines.append(line);
}

void IoT_Network_Scene::draw_an_arrow(QPointF a, QPointF b,QColor color,unsigned int index)
{
	MyLine * newline = new MyLine(a,b,1,color,index);
	this->addItem(newline);
	GlobalLineVector.push_back(newline);
	//this->add_a_line(newline);
	this->update();
}
void IoT_Network_Scene::draw_an_arrow_Consensus(QPointF a, QPointF b, QColor color, unsigned index)
{
	MyLine * newline = new MyLine(a, b, 1, color, index);
	this->addItem(newline);
	GlobalLineVector.push_back(newline);
	//this->add_a_line(newline);
	this->update();
}

void IoT_Network_Scene::draw_an_item(QPointF pos)
{
	MyItem* newItem = new MyItem(node_index++, false,pos);
	newItem->set_r(10);
	connect(newItem, SIGNAL(askSceneDelete(MyItem*)), this, SLOT(SlotDeleteItem(MyItem*)));
	connect(newItem->receiver, SIGNAL(toSceneUpdateText(unsigned int, QString)), this, SLOT(SlotUpdateLog(unsigned int, QString)));
	connect(newItem->receiver, SIGNAL(toScene(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int)), this, SLOT(Scene_act_as_asked(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int)));
	connect(newItem->receiver, SIGNAL(toSceneDeleteLines()), this, SLOT(SlotDeleteLineVector()));
	connect(newItem->receiver, SIGNAL(toSceneDeleteLine(unsigned int)), this, SLOT(SlotDeleteLine(unsigned int)));

	connect(newItem, SIGNAL(toSceneUpdateRange(int)), this, SLOT(SlotUpdateRange(int)));
	this->addItem(newItem);
	this->addItem(newItem->range);
	//GlobalTextVector.append(newItem->Text);
	this->addItem(newItem->Text);
	GlobalItemVector.push_back(newItem);
	//this->graphics_items.append(newItem);
	this->update();
}
void IoT_Network_Scene::draw_an_item()
{
	MyItem* newItem = new MyItem(node_index++, true);
	connect(newItem, SIGNAL(askSceneDelete(MyItem*)), this, SLOT(SlotDeleteItem(MyItem*)));
	connect(newItem->receiver, SIGNAL(toSceneUpdateText(unsigned int, QString)), this, SLOT(SlotUpdateLog(unsigned int, QString)));
	connect(newItem->receiver, SIGNAL(toScene(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int)), this, SLOT(Scene_act_as_asked(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int)));
	connect(newItem->receiver, SIGNAL(toSceneDeleteLines()), this, SLOT(SlotDeleteLineVector()));
	connect(newItem->receiver, SIGNAL(toSceneDeleteLine(unsigned int)), this, SLOT(SlotDeleteLine(unsigned int)));
	/*connect(newItem->consensus, SIGNAL(toSceneDrawArrow(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int index)), this, SLOT(Scene_act_as_asked(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int)));*/
	connect(newItem->consensus, SIGNAL(toSceneDeleteLine(unsigned int)), this, SLOT(SlotDeleteLine(unsigned int)));
	connect(newItem, SIGNAL(toSceneUpdateRange(int)), this, SLOT(SlotUpdateRange(int)));
	this->addItem(newItem);
	this->addItem(newItem->range);
	//GlobalTextVector.append(newItem->Text);
	this->addItem(newItem->Text);
	GlobalItemVector.push_back(newItem);
	//this->graphics_items.append(newItem);
	this->update();
}
void IoT_Network_Scene::SlotDeleteItem(MyItem* item)
{
	removeItem(item);
	//GlobalItemVector.remove(item->getIndex());
	GlobalItemVector[item->getIndex()]->setActive(false);
	//delete item;
}
void IoT_Network_Scene::SlotDeleteLineVector()
{
	for (int i=0;i<GlobalLineVector.size();i++)
	{
		removeItem(GlobalLineVector[i]);
		delete GlobalLineVector[i];
		GlobalLineVector[i] = nullptr;
	}
	GlobalLineVector.clear();
}

void IoT_Network_Scene::SlotDeleteLine(unsigned index)
{
	for (int i=GlobalLineVector.size()-1;i>=0;i--)
	{
		if (GlobalLineVector[i]->getIndex()==index)
		{
			removeItem(GlobalLineVector[i]);
			delete GlobalLineVector[i];
			GlobalLineVector[i] = nullptr;
			GlobalLineVector.erase(GlobalLineVector.begin()+i);
		}
	}
	if (GlobalLineVector.empty())
	{
		qDeleteAll(GlobalLineVector);
		GlobalLineVector.clear();
	}
}

void IoT_Network_Scene::SlotUpdateLog(unsigned int index, QString updateString)
{
	GlobalTextVector[index]->setPlainText(updateString);
}
void IoT_Network_Scene::SlotUpdateRange(int index)
{
	this->addItem(GlobalItemVector[index]->range);
}
