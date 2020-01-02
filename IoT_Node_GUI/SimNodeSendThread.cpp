#include "SimNodeSendThread.h"
#include <QtNetwork/QHostAddress>
#include <jsoncpp/json.h>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
#include "TextItem.h"
#include "myline.h"
#include "myitem.h"
#include "util.h"

extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
extern unsigned int node_index;
//To simulate the range of broadcast, multi-cast is not used, this thread uses uni-cast to control
//each message, if the destination is out of range, the message will not be sent.
SimNodeSendThread::SimNodeSendThread(unsigned index, uint16_t port)
{
	this->index = index;
	this->port = port;
	this->sender = new QUdpSocket(this);
	sender->bind(QHostAddress::LocalHost, port);
	srand(time(0)+rand());
}
unsigned SimNodeSendThread::getIndex()
{
	return this->index;
}
uint16_t SimNodeSendThread::getPort()
{
	return this->port;
}
void SimNodeSendThread::setIndex(unsigned index)
{	
	this->index = index;
}

void SimNodeSendThread::setPort(uint16_t port)
{
	this->port = port;
}
 void SimNodeSendThread::setFrequency(unsigned frequency)
 {
	 this->frequency = frequency;
 }
void SimNodeSendThread::setSpeed(unsigned speed)
{
	this->speed = speed;
}

void SimNodeSendThread::setDataSize(unsigned size)
{
	this->dataSize = size;
}
void SimNodeSendThread::stop()
{
	this->stopFlag = true;
}



//This thread is used to simulator the upload of data, data is a random string, this thread uses a
//random function to set the frequency of data uploads. If the parameter 'frequency' is larger,
//this thread uploads data less frequently. The data is uploaded in JSON, which contains timestamp.
//data uploader(owner, use index to represent), signature(use port to represent the signature)
//and a random string(to represent the data)
void SimNodeSendThread::run()
{
	Json::Value Root;
	Json::FastWriter writer;
	stopFlag = false;
	while (!stopFlag)
	{

		msleep(speed);
		if (rand()%(frequency+index)!=0)
		{
			continue;	
		}
		Util util;
		std::string data = util.utilRandomString(dataSize);
		//Then package this data
		std::string localtime = util.utilLocalTimeStr();
		Root.clear();
		Root["TIME"] = localtime;
		Root["UPLOADER"] = index;
		Root["SIGNATURE"] = port;
		Root["CONTEXT"] = data;
		std::string dataPackage = writer.write(Root);
		emit toDBAddLocal(QString::fromStdString( localtime),QString::fromStdString( data));
		//Send this msg to every node in the range
		for (int i=0; i<GlobalItemVector.size();i++)
		{

			int x2 = GlobalItemVector[i]->getCentre().x();
			int y2 = GlobalItemVector[i]->getCentre().y();
			int x1 = GlobalItemVector[index]->x();
			float t = util.calculateDistance(GlobalItemVector[index]->x(), GlobalItemVector[index]->y(), x2, y2);
			if (t<=GlobalItemVector[index]->getRadius())
			{
				//if destination is in transportation radius
				quint16 port = DataNetworkStartPort + GlobalItemVector[i]->getIndex();
				int result = sender->writeDatagram(dataPackage.c_str(), dataPackage.length(), QHostAddress::Broadcast, port);
			}
		}
		
		
		
	}
}
void SimNodeSendThread::close()
{
	this->sender->close();
}

