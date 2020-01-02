#ifndef MYLINE_H
#define MYLINE_H
#include <QGraphicsItem>
#include <QPoint>
class MyLine :public QGraphicsItem
{
	
public:
	MyLine();
	MyLine(QPointF p1,QPointF p2,unsigned int type, QColor colour,unsigned int index);//用来绘制两点之间的连线
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	void setLocation(QPointF location1, QPointF location2);
	void setType(unsigned int a);
	unsigned int getIndex();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
private:
	QColor color;
	QPointF last_point;
	QPointF location1;
	QPointF location2;
	unsigned int type;// 0 is line, 1 is arrow
	unsigned int index;//which item does this line belongs to
};


#endif // MYLINE
