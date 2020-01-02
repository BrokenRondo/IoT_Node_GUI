#include <QTimer>
#include "SimNodeRecvThread.h"
#include "jsoncpp/json.h"
#include <queue>
#include <mutex>
extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
extern std::queue<QTimer*>GlobalTimerVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
extern unsigned int node_index;
SimNodeRecvThread::SimNodeRecvThread(unsigned index, uint16_t port,NodeBlockchainDatabase* DB)
{
	this->index = index;
	this->port = port;
	receiver = new QUdpSocket(this);
	receiver->bind( (quint16)port,QUdpSocket::ShareAddress);
	connect(receiver, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	localDatabase = DB;
	//localLog = new NodeLog(index);
	//QTimer *timer=new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerUpdate()));
	//timer->setSingleShot(false);
	//timer->start(100);
	//GlobalTimerVector.enqueue(timer);
}
unsigned SimNodeRecvThread::getIndex()
{
	return this->index;
}
uint16_t SimNodeRecvThread::getPort()
{
	return this->port;
}
void SimNodeRecvThread::setIndex(unsigned index)
{
	this->index = index;
}

void SimNodeRecvThread::setPort(uint16_t port)
{
	this->port = port;
}

void SimNodeRecvThread::readPendingDatagrams()
{
	while (receiver->hasPendingDatagrams())
	{
		char buf[1024] = { 0 };
		QHostAddress addr;
		quint16 port;
		qint64 len;
		QString recv_addr;
		QString recv_port;
		QString recv_time;
		QString msgString = "";
		len = receiver->readDatagram(buf, sizeof(buf), &addr, &port);
		if (len > 0)
		{
			recv_addr = addr.toString();
			recv_port = QString::number(port);
		}
		else
		{
			msgString = "Received Wrong Data, Error code:0x00000001.\n";
			emit toGUIAddLog(msgString);
			GlobalLogVector[index].append(msgString);
			return;
		}
		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(buf, root, false))
		{

			msgString = tr("Received Wrong Data From %1:%2, Cannot Parse!\n").arg(addr.toString()).arg(port);
			emit toGUIAddLog(msgString);
			GlobalLogVector[index].append(msgString);
			return;
		}
		if (root["UPLOADER"].asInt() != port - DataNetworkStartPort || root["SIGNATURE"].asInt() != port)
		{
			msgString = tr("Received Bad Data! Signature not valid, from %1.").arg(port);
			GlobalLogVector[index].append( msgString);
			return;
		}
		recv_time = QString::fromStdString(root["TIME"].asString());
		msgString = tr("Received Data { %1 } From %2: %3\n").arg(QString::fromStdString( root["CONTEXT"].asString())).arg(port).arg(recv_time);
		//emit toGUIAddLog(msgString);
		AddLog(msgString);
		if (GlobalLogVector.size()>0) 
		{
			if (GlobalLogVector[index].length() > 500)
			{
				GlobalLogVector[index].clear();//To avoid string too long
			}
		}
		src = root["UPLOADER"].asInt();
		
		GlobalLogVector[index].append(msgString);
		emit toSceneUpdateText(index, GlobalLogVector[index]);
		
		QPointF a = GlobalItemVector[src]->getCentre();
		emit toScene(1, nullptr, nullptr, a, GlobalItemVector[index]->getCentre(), a, a, Qt::blue,index);
		QTimer *timer = new QTimer();
		timer->setSingleShot(true);
		timer->start(200);
		GlobalTimerVector.push(timer);
		connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerUpdate()));
		//
		//QTimer::singleShot(100, this, SLOT(slotTimerUpdate()));
		//emit toDBAddMsg(recv_time, recv_addr, recv_port, QString::fromStdString(root["CONTEXT"].asString()));
		AddMsg(recv_time, recv_addr, recv_port, QString::fromStdString(root["CONTEXT"].asString()));
		
	}
}
void SimNodeRecvThread::slotTimerUpdate()
{
	emit toSceneDeleteLine(this->index);
}

void SimNodeRecvThread::run()
{

}
void SimNodeRecvThread::close()
{
	this->receiver->close();
}
void SimNodeRecvThread::AddMsg(QString timestamp, QString uploader, QString signature, QString context)
{
	this->localDatabase->UpdateMsgDB(timestamp,uploader,signature,context);
}
void SimNodeRecvThread::AddLocalData(QString timestamp, QString context)
{
	this->localDatabase->UpdateLocalDB(timestamp, context);
}
void SimNodeRecvThread::AddLog(QString str)
{
	this->localDatabase->UpdateLog(str);
}
void SimNodeRecvThread::showDB()
{
	//localDatabase->setWindowFlags(Qt::WindowStaysOnTopHint);
	this->localDatabase->show();
}
void SimNodeRecvThread::UpdateLocalDB(QString timestamp, QString context)
{
	this->localDatabase->UpdateLocalDB(timestamp, context);
}
void SimNodeRecvThread::toSceneDrawArrow(QPointF src, QPointF dst, QColor color, unsigned index)
{
	srand(time(nullptr) + index);
	QTimer *timer = new QTimer();
	timer->setSingleShot(true);
	connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerUpdate()));
	if (color==Qt::red)
	{
		timer->start(200);
	}
	else
	{
		timer->start( 200);
	}

	GlobalTimerVector.push(timer);

	//QTimer::singleShot(100, this, SLOT(slotTimerUpdate()));
	emit toScene(1, nullptr, nullptr, src, dst, src, src, color, index);
}

