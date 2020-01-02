#pragma once
#ifndef BROADCASTTHREAD
#define BROADCASTTHREAD
#include <QThread>
#include <string>
#include <QtNetwork/QtNetwork>

class BroadcastThread:public QThread
{
public:
	BroadcastThread(int waitspeed,int index);
	void run() override;
	void setSpeed(int speed);
	void broadcast(std::string str);
	void stop();
private:
	bool stopflag;
	int waitspeed=300;
	QUdpSocket *broadcaster;
	unsigned int index;
	std::vector<std::string> waitlist;
};



#endif
