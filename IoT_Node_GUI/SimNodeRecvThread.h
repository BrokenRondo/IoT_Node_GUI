#pragma once
#ifndef SIM_NODE_RECV_THREAD
#define SIM_NODE_RECV_THREAD
#include <QThread>
#include <QString>
#include <QtNetwork/QtNetwork>
#include "myitem.h"
#include "myline.h"
#include "NodeBlockchainDatabase.h"
#include "NodeLog.h"

class MyItem;
class MyLine;
//This Thread aims to simulate the receive action of a node, the send and receive communication
//is simulated by socket, the communication arrow showed in QGraphicsView will only be drawn when
//receives a message
class SimNodeRecvThread:public QThread
{
	Q_OBJECT
public:
	SimNodeRecvThread(unsigned int index, uint16_t port,NodeBlockchainDatabase* DB);
	void stop();
	void setPort(uint16_t port);
	void setIndex(unsigned int index);
	uint16_t getPort();
	unsigned int getIndex();
	void close();
	void AddMsg(QString, QString, QString, QString);
	void AddLocalData(QString, QString);
	void AddLog(QString str);
	void toSceneDrawArrow(QPointF src, QPointF dst, QColor color, unsigned int index);
	NodeBlockchainDatabase* localDatabase;
signals:
	void toUpdateNodeLog(unsigned int, QString );
	void toScene(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor,unsigned int index);
	void toSceneUpdateText(unsigned int, QString);
	void toSceneDeleteLines();
	void toSceneDeleteLine(unsigned int);
	void toGUIAddLog(QString);
	void toDBAddMsg(QString, QString, QString, QString);



protected:
	void run() override;
private:
	uint16_t port = 0;  //socket port of this thread
	unsigned int src = 0;// who sent the message
	unsigned int index = 0;//to match the same index of the node
	bool stopFlag = false;
	QUdpSocket *receiver;
	unsigned int frequency=100;
	

private slots:
	void readPendingDatagrams();
	void slotTimerUpdate();
	void showDB();
	void UpdateLocalDB(QString, QString);
	
};

#endif

