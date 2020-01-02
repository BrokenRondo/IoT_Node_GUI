#ifndef MYITEM_H
#define MYITEM_H
#include <QGraphicsItem>
#include <QPoint>
#include "RangeItem.h"
#include "TextItem.h"
#include "SimNodeRecvThread.h"
#include "SimNodeSendThread.h"
#include "NodeBlockchainDatabase.h"
#include "NodeLog.h"
#include "Consensus.h"


class SimNodeRecvThread;
class SimNodeSendThread;
class Consensus;
class MyItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	MyItem(unsigned int index,unsigned int r);
	MyItem(unsigned index, bool RandomorNot, QPointF pos);
	MyItem(unsigned index, bool RandomorNot);
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	QPointF getLastPoint();
	unsigned int getIndex();
	void updatePos(QPointF a);//to update the pos if this item is setpos in a scene
	void updatePos(qreal x, qreal y);
	void set_r(unsigned r);//set the r of a circle
	QPointF getCentre();//get the centre of the item if it's a circle
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
	void setActive(bool);
	bool getActive();
	unsigned int getRadius();
	void setType(unsigned int);
	RangeItem * range;// to show the transmission range of this item
	TextItem * Text;//Textbox to show the log
	SimNodeSendThread *sender;
	SimNodeRecvThread *receiver;
	NodeBlockchainDatabase* nodeBlockchainDatabase;
	NodeLog* nodelog;
	Consensus *consensus;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
	QColor color;
	QPointF last_point;
	unsigned int index;
	QPoint Centre;
	
	unsigned int r = 6;//default radius
	bool active = true;//if this item is active
	unsigned int rangeRadius=800;// communication range
	
	unsigned int type = 0;// 0 is end node, 1 is edge node, default is end node
public slots:
	void slotRemoveItem();
	void ShowRange();
	void HideRange();
	void ChangeRangeTriggered();
	void ShowBCDatabase();
	void ShowLog();

signals:
	void askSceneDelete(MyItem* toDeleteItem);
	void toSceneUpdateRange(int index);

};


#endif // MYITEM_H
