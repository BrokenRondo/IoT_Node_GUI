#include "RangeItem.h"
#include <QPainter>
RangeItem::RangeItem(QPointF centre, unsigned radius, QColor colour)
{
	this->centre = centre;
	x = centre.x();
	y = centre.y();
	this->radius = radius;
	this->color = colour;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}
RangeItem::RangeItem(int x, int y, unsigned int r, QColor color)
{
	this->x = x;
	this->y = y;
	this->radius = r;
	this->color = color;
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}


void RangeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QPen(color, 2, Qt::DotLine));
	if (this->radius>=1000)
	{
		painter->setPen(QPen(color, 4, Qt::DotLine));
	}
	if (this->radius>=2000)
	{
		painter->setPen(QPen(color, 8, Qt::DotLine));
	}
	if (this->radius>=4000)
	{
		painter->setPen(QPen(color, 10, Qt::DotLine));
	}
	painter->drawEllipse(centre, radius, radius);
	
}
QRectF RangeItem::boundingRect() const
{
	qreal penWidth = 1;

	return QRectF(0 - penWidth / 2, 0 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}
void RangeItem::fatheritemMove(QGraphicsSceneMouseEvent event)
{
	
}
void RangeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseMoveEvent(event);
}
void RangeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mousePressEvent(event);
}
void RangeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}


