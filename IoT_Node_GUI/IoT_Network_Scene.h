#pragma once
#ifndef IOT_N_V
#define IOT_N_V
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStack>
#include "myitem.h"
#include "myline.h"
class IoT_Network_Scene:public QGraphicsScene
{
	Q_OBJECT
public:
	void draw_an_arrow(QPointF a, QPointF b,QColor color,unsigned int index);
	void draw_an_arrow_Consensus(QPointF a, QPointF b, QColor color, unsigned int index);
	void add_an_item(QGraphicsItem* item);
	void add_a_line(QGraphicsItem* line);
	void draw_an_item(QPointF pos);
	void draw_an_item();
public slots:
	void Scene_act_as_asked(unsigned int, MyItem* , MyLine*e, QPointF , QPointF , QPointF , QPointF, QColor , unsigned int);
	void SlotDeleteItem(MyItem*);
	//void SlotDeleteItemVector(QVector<MyItem*>);//To delete all the items in QVector
	void SlotDeleteLineVector();//To delete all the lines in QVector
	void SlotUpdateLog(unsigned int index,QString updateString);//To update log for each item
	void SlotDeleteLine(unsigned int index);//To delete the line which belongs to item(index)
	void SlotUpdateRange(int index);

private:
	QVector<QGraphicsItem *> graphics_items;
	QStack<QGraphicsItem* > graphics_lines;
	
};
#endif
