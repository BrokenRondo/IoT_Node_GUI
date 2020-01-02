#pragma once
#ifndef ACT_THREAD
#define ACT_THREAD
#include <QThread>
#include <QPointF>
#include <QString>
#include <QGraphicsview>
#include "myline.h"
#include "myitem.h"

class actThread:public QThread
{
	Q_OBJECT
public:
	void setMainView(QGraphicsView *view);
	void setStop(bool stop);
protected:
	void run() override;

signals:
		void toScene(unsigned int, MyItem* , MyLine*, QPointF, QPointF, QPointF, QPointF, QColor);
		void toSceneDeleteLines();
		void toSceneUpdateText(unsigned int, QString);
private:
	QGraphicsView * mainView=nullptr;
	bool stop = false;
};

#endif
