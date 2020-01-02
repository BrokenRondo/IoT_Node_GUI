#include "myitem.h"
#include<QPainter>
#include <QCursor>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMenu>
#include <QGraphicsItem>
#include <string>
#include <ctime>
#include <QTextBlockFormat>
#include <QTextCursor>
#include "myline.h"
#include "ChangeRangeDialog.h"

using namespace::std;
extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
extern int fieldSize;
extern int rangeSize;
extern unsigned int node_index;

MyItem::MyItem(unsigned int index,unsigned int r)
{
	srand((unsigned)time(0));
	rangeRadius = rangeSize;
	setToolTip("Click and drag me!");
	this->index = index;
	this->r = r;
	setCursor(Qt::OpenHandCursor);
	color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->last_point = this->pos();
	RangeItem* sonItem = new RangeItem(this->pos().x(),this->pos().y(), this->rangeRadius, this->color);
	this->range = sonItem;
	this->range->setOpacity(0);
	//this->range->setVisible(false);
	TextItem* textItem = new TextItem(this->pos().x(), this->pos().y(), this->r, this->color);
	//textItem->setTextWidth(textItem->boundingRect().width());
	textItem->setPlainText(QString("LOG:"));
	GlobalTextVector.push_back(textItem);
	GlobalLogVector.push_back("LOG:\n");
	this->Text = textItem;
	nodeBlockchainDatabase = new NodeBlockchainDatabase(index);
	sender = new SimNodeSendThread(this->index, DataNetworkStartPort + this->index);
	receiver = new SimNodeRecvThread(this->index, DataNetworkStartPort + this->index,nodeBlockchainDatabase);
	consensus = new Consensus(index, ConsensusStartPort + index, sender, receiver);

}
MyItem::MyItem(unsigned index, bool RandomorNot, QPointF pos)
{
	rangeRadius = rangeSize;
	srand((unsigned)time(0) + index);
	setToolTip("Click and drag me!");
	this->index = index;
	setCursor(Qt::OpenHandCursor);
	color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->last_point = this->pos();
	if (RandomorNot)
	{
		srand((unsigned)time(0) + rand());
		qreal x = (int)(pow(-1, rand())* rand()) % fieldSize;
		qreal y = (int)(pow(-1, rand())* rand()) %fieldSize ;
		this->setPos(QPointF(x, y));
	}
	else
	{
		this->setPos(pos);
	}
	RangeItem* sonItem = new RangeItem(this->pos(), this->rangeRadius, this->color);
	this->range = sonItem;
	this->range->setOpacity(0);
	//this->range->setVisible(false);
	TextItem* textItem = new TextItem(this->pos().x(), this->pos().y(), this->r, Qt::black);
	textItem->setPlainText(QString("LOG:"));
	GlobalTextVector.push_back(textItem);
	GlobalLogVector.push_back("LOG:\n");
	this->Text = textItem;
	nodeBlockchainDatabase = new NodeBlockchainDatabase(index);
	nodelog = new NodeLog(index);
	sender = new SimNodeSendThread(this->index, DataNetworkStartPort + this->index);
	receiver = new SimNodeRecvThread(this->index, DataNetworkStartPort + this->index,nodeBlockchainDatabase);
	connect(this->receiver, SIGNAL(toGUIAddLog(QString)), nodelog, SLOT(addLog(QString)));
	consensus = new Consensus(index, ConsensusStartPort + index, sender, receiver);
}
MyItem::MyItem(unsigned index, bool RandomorNot)
{
	srand((unsigned)time(0)+index);
	rangeRadius = rangeSize;
	setToolTip("Click and drag me!");
	this->index = index;
	setCursor(Qt::OpenHandCursor);
	color = QColor(qrand() % 256, qrand() % 256, qrand() % 256);
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	this->last_point = this->pos();
	if (RandomorNot)
	{
		srand((unsigned)time(0)+rand());
		qreal x = (int)(pow(-1, rand())* rand()) % fieldSize;
		qreal y = (int)(pow(-1, rand())* rand()) % fieldSize;
		this->setPos(QPointF(x, y));
	}
	RangeItem* sonItem = new RangeItem(this->pos(), this->rangeRadius, this->color);
	this->range = sonItem;
	this->range->setOpacity(0);
	//this->range->setVisible(false);
	TextItem* textItem = new TextItem(this->pos().x(), this->pos().y(), this->r, Qt::black);
	textItem->setPlainText(QString("LOG:"));
	GlobalTextVector.push_back(textItem);
	GlobalLogVector.push_back("LOG:\n");
	this->Text = textItem;
	nodeBlockchainDatabase = new NodeBlockchainDatabase(index);
	nodelog = new NodeLog(index);
	sender = new SimNodeSendThread(this->index, DataNetworkStartPort + this->index);
	receiver = new SimNodeRecvThread(this->index, DataNetworkStartPort + this->index,nodeBlockchainDatabase);
	consensus = new Consensus(index, ConsensusStartPort + index, sender, receiver);


	connect(this->sender, SIGNAL(toDBAddLocal(QString, QString)), this->receiver, SLOT(UpdateLocalDB(QString, QString)));
}

unsigned int MyItem::getIndex()
{
	return this->index;
}

unsigned MyItem::getRadius()
{
	return this->rangeRadius;
}


void MyItem::set_r(unsigned r)
{
	this->r = r;
}
void MyItem::setActive(bool flag)
{
	this->active = flag;
}

bool MyItem::getActive()
{
	return this->active;
}

QPointF MyItem::getLastPoint()
{
	return  this->scenePos();
}
QPointF MyItem::getCentre()
{
	qreal rr = this->r;
	return QPointF(this->scenePos().x() , this->scenePos().y() );
}

void MyItem::updatePos(QPointF a)
{
	this->last_point = this->scenePos();
}
void MyItem::updatePos(qreal x, qreal y)
{
	this->last_point = this->scenePos();
}

QRectF MyItem::boundingRect() const
{
	qreal penWidth = 1;
	
	return QRectF(0-penWidth/2,0-penWidth/2,20+penWidth,20+penWidth);
}
void MyItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);//表明这个参数没有使用
	Q_UNUSED(widget);
	painter->setBrush(color);
	painter->drawEllipse(QPointF(0,0), 2*r, 2*r);
	if (this->index<10)
	{
		painter->drawText(QPointF(-3, 4), QString::number(this->index));
	}
	else
	{
		painter->drawText(QPointF(-6, 4), QString::number(this->index));
	}
}
void MyItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{

	if(event->button()!=Qt::LeftButton)//只有当鼠标左键点击时才响应
	{
		event->ignore();
		return;
	}
	this->range->setSelected(true);
	this->Text->setSelected(true);
	this->range->mousePressEvent(event);
	setCursor(Qt::ClosedHandCursor);
}


void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	//this->range->mouseReleaseEvent(event);
	this->range->setSelected(false);
	this->Text->setSelected(false);
	this->setSelected(false);
	setCursor(Qt::OpenHandCursor);
}

void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

	QGraphicsItem::mouseMoveEvent(event);
	QPointF p=this->scenePos();
	this->last_point = p;
}
 void MyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
 {
	QMenu menu;
	QAction *removeAction = menu.addAction("Remove");
	QAction *selectedAction = menu.addAction("Select");
	QAction *showRange = menu.addAction("ShowRange");
	QAction *HideRange = menu.addAction("HideRange");
	QAction *changeRange = menu.addAction("ChangeRange");
	QAction *showBlockchainData = menu.addAction("ShowBlockchainData");
	//QAction *showLog = menu.addAction("ShowLog");
	connect(removeAction, SIGNAL(triggered()), this, SLOT(slotRemoveItem()));
	connect(showRange, SIGNAL(triggered()), this, SLOT(ShowRange()));
	connect(HideRange, SIGNAL(triggered()), this, SLOT(HideRange()));
	connect(changeRange, SIGNAL(triggered()), this, SLOT(ChangeRangeTriggered()));
	connect(showBlockchainData, SIGNAL(triggered()), this->receiver, SLOT(showDB()));
	//connect(showLog, SIGNAL(triggered()), this, SLOT(ShowLog()));
	menu.exec(event->screenPos());

 }
void MyItem::slotRemoveItem()
{
	emit askSceneDelete(this);
}


void MyItem::setType(unsigned int type)
{
	if (type==0)
	{
		this->type = 0;
	}
	else if	(type==1)
	{
		this->type = 1;
	}
}
void MyItem::ShowRange()
{
	//this->range->setVisible(true);
	this->range->setOpacity(1);
}
void MyItem::HideRange()
{
	//this->range->setVisible(false);
	this->range->setOpacity(0);
}
void MyItem::ChangeRangeTriggered()
{
	ChangeRangeDialog dialog(nullptr);
	dialog.exec();
	int newRangeRadius = dialog.getInput();
	delete this->range;
	this->range = nullptr;
	this->rangeRadius = newRangeRadius;
	RangeItem* sonItem = new RangeItem(this->pos(), newRangeRadius, this->color);
	this->range = sonItem;
	this->range->setOpacity(1);
	emit toSceneUpdateRange(index);
}
void MyItem::ShowBCDatabase()
{
	this->nodeBlockchainDatabase->show();
}
void MyItem::ShowLog()
{
	this->nodelog->show();
}



