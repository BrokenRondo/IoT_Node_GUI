#pragma once
#ifndef SIM_NODE_SEND_THREAD
#define SIM_NODE_SEND_THREAD
#include <QThread>
#include <QtNetwork/QtNetwork>

//This Thread aims to simulate the send action of a node, the send and receive communication
//is simulated by socket, note, the communication arrow showed in QGraphicsView will only be
//drawn when receives a message
class SimNodeSendThread:public QThread
{
	Q_OBJECT
public:
	SimNodeSendThread(unsigned int index, uint16_t port);
	void stop();
	void setPort(uint16_t port);
	void setIndex(unsigned int index);
	uint16_t getPort();
	unsigned int getIndex();
	void setFrequency(unsigned int frequency);
	void setSpeed(unsigned int speed);
	void setDataSize(unsigned int size);
	void close();
	
	
signals:
	void toDBAddLocal(QString, QString);
	
protected:
	void run() override;
private:
	uint16_t port = 0;  //socket port of this thread
	unsigned int index = 0;//to match the same index of the node
	bool stopFlag = false;
	QUdpSocket *sender;
	unsigned int frequency=100;//use this to set the frequency of data uploads, the larger, the slower.
	unsigned int speed = 1000;// use this to set the sleep time (millisecond) of the data upload thread;
	unsigned int dataSize = 20;//use this to set the size of the data crated to be updated.(Byte)
};





#endif