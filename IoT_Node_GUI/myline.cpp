#include "myitem.h"
#include<QPainter>
#include <QCursor>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPixmap>
#include "myline.h"
extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
extern QQueue<QTimer*> GlobalTimerVector;
MyLine::MyLine()
{
	setToolTip("Click and drag me!");
	setCursor(Qt::OpenHandCursor);
	color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
	//setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}
MyLine::MyLine(QPointF location1, QPointF location2, unsigned int type, QColor colour,unsigned int index)
{
	this->location1 = location1;
	this->location2 = location2;
	this->color = colour;
	this->type = type;
	this->index = index;
}
void MyLine::setType(unsigned a)
{
	this->type = a;
}
unsigned MyLine::getIndex()
{
	return this->index;
}


QRectF MyLine::boundingRect() const
{
	qreal penWidth = 1;

	return QRectF(0 - penWidth / 2, 0 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}
void MyLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);//表明这个参数没有使用
	Q_UNUSED(widget);
	QPen myPen(color, 1, Qt::DotLine);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(myPen);
	if (this->type==0)
	{
		painter->drawLine(location1, location2);
	}
	else if(this->type==1)
	{

		painter->drawLine(location1, location2);
		qreal alph = atan2(location2.y() - location1.y(), location2.x() - location1.x());
		painter->translate(location2);

		qreal angle = (alph * 180) / 3.14159;
		painter->rotate(angle);
		myPen.setStyle(Qt::SolidLine);
		painter->setPen(myPen);
		painter->drawLine(QPointF(-15, -8), QPointF(0, 0));
		painter->drawLine(QPointF(-15, +8), QPointF(0, 0));
		
	}
	

}
void MyLine::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)//只有当鼠标左键点击时才响应
	{
		event->ignore();
		return;
	}
	setCursor(Qt::ClosedHandCursor);
}


void MyLine::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	setCursor(Qt::OpenHandCursor);
}

void MyLine::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

	QGraphicsItem::mouseMoveEvent(event);
	QPointF p = this->scenePos();
	this->last_point = p;


}

void MyLine::setLocation(QPointF location1, QPointF location2)
{
	this->location1 = location1;
	this->location2 = location2;
}

