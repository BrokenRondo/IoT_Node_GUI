#include "TextItem.h"
#include <QPainter>
TextItem::TextItem()
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->setTextInteractionFlags(Qt::TextEditorInteraction);
}
TextItem::TextItem(int x, int y, unsigned radius,QColor color)
{
	this->x = x;
	this->y = y;
	this->radius = radius;
	this->color = color;
	this->setPos(x-220 , y+ 25);
	this->setTextWidth(440);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->setTextInteractionFlags(Qt::TextEditorInteraction);
}
QRectF TextItem::boundingRect() const
{
	qreal penWidth = 1;

	return QRectF(-0 - penWidth / 2,  3- penWidth / 2, 440 + penWidth, 150 + penWidth);
}
void TextItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseMoveEvent(event);
}
void TextItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mousePressEvent(event);
}
void TextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}


//void TextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//{
//	painter->setPen(QPen(color, 1, Qt::DashDotLine));
//	painter->drawRect(x-200, y+10, 390 + 2 * radius, 70);
//	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
//	painter->drawText(x - 200, y + 20,QString("LOG"));
//	
//}





