#pragma once
#ifndef R_ITEM
#define R_ITEM
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
class RangeItem: public QGraphicsItem
{
public:
	RangeItem();
	RangeItem(QPointF centre, unsigned int radius, QColor color);
	RangeItem(int x, int y, unsigned int radius, QColor color);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	QRectF boundingRect() const override;
	void fatheritemMove(QGraphicsSceneMouseEvent event);
private:
	QPointF centre;
	int x=0, y = 0;
	unsigned int radius;
	QColor color=Qt::blue;
};




#endif
