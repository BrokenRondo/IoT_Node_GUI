#include <QMutex>
#include "broadcastThread.h"
#include "myitem.h"
#include "util.h"

extern std::vector<MyItem*> GlobalItemVector;
const int ConsensusStartPort = 60000;
const int ConsensusProposePort = 40000;
const int DataNetworkStartPort = 50000;
extern unsigned int node_index;
BroadcastThread::BroadcastThread(int waitspeed, int index)
{
	this->waitspeed = waitspeed;
	this->index = index;
	broadcaster = new QUdpSocket(nullptr);
	this->broadcaster->bind(QHostAddress::LocalHost, index + ConsensusProposePort, QAbstractSocket::ReuseAddressHint);
	
}
void BroadcastThread::broadcast(std::string str)
{
	QMutex mutex;
	mutex.lock();
	this->waitlist.push_back(str);
	mutex.unlock();
	
}
void BroadcastThread::stop()
{
	this->stopflag = true;
}
void BroadcastThread::setSpeed(int speed)
{
	this->waitspeed = speed;
}

void BroadcastThread::run()
{
	Util util;
	stopflag = false;
	while (!stopflag)
	{
		while (waitlist.size()>0)
		{
			for (int i = 0; i < GlobalItemVector.size(); i++)
			{
				int x2 = GlobalItemVector[i]->getCentre().x();
				int y2 = GlobalItemVector[i]->getCentre().y();
				int x1 = GlobalItemVector[index]->x();
				float t = util.calculateDistance(GlobalItemVector[index]->x(), GlobalItemVector[index]->y(), x2, y2);
				if (t <= GlobalItemVector[index]->getRadius())
				{
					quint16 destPort = ConsensusStartPort + GlobalItemVector[i]->getIndex();
					msleep(waitspeed);
					int result = broadcaster->writeDatagram(waitlist.back().c_str(), waitlist.back().length(), QHostAddress::LocalHost, destPort);
	
				}
			}
			waitlist.pop_back();
		}
		msleep(200);
	}

}


