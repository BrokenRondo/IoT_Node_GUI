#pragma once
#ifndef T_ITEM
#define T_ITEM
#include <QWidget>
#include <QGraphicsItem>
#include <QObject>
class TextItem:public QGraphicsTextItem
{
public:
	TextItem();
	TextItem(int x, int y, unsigned int radius,QColor color);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	//void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	QRectF boundingRect() const override;

private:
	int x=0;
	int y=0;
	int radius=12;
	QColor color = Qt::black;
};




#endif
