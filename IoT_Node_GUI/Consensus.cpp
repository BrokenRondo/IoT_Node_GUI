#include "Consensus.h"
#include <cmath>
#include <algorithm>
#include "util.h"
#include "TextItem.h"
#include "myline.h"
#include "jsoncpp/json.h"

extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
const int ConsensusProposePort = 40000;
extern unsigned int node_index;
extern time_t start;
extern time_t end;
extern unsigned int ConsensusGap;
struct MsgCount
{
	QString msg = "";
	int count = 0;
	unsigned int view = 0;
	unsigned int round = 0;
	bool operator==(const MsgCount &msgCount)
	{
		return (msg == msgCount.msg&&view==msgCount.view&&round==msgCount.round);
	}
};
Consensus::Consensus(unsigned index, uint16_t port)
{
	this->index = index;
	this->port = port;
	this->n = GlobalItemVector.size();
	this->f = floor(GlobalItemVector.size() / 3);
	this->udpsocket = new QUdpSocket(nullptr);
	udpsocket->bind(QHostAddress::LocalHost, port,QAbstractSocket::ReuseAddressHint);
	this->broadcast_ = new BroadcastThread(200, index);
	connect(udpsocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}
Consensus::Consensus(unsigned index)
{
	this->index = index;
	this->port = ConsensusStartPort+index;
	this->n = GlobalItemVector.size();
	this->f = floor(GlobalItemVector.size() / 3);
	this->udpsocket = new QUdpSocket(this);
	udpsocket->bind(QHostAddress::LocalHost, port, QAbstractSocket::ReuseAddressHint);
	this->broadcast_ = new BroadcastThread(200, index);
	connect(udpsocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}
Consensus::Consensus(unsigned index, uint16_t port,SimNodeSendThread *sender, SimNodeRecvThread *receiver)
{
	this->index = index;
	this->port = port;
	this->n = GlobalItemVector.size();
	this->f = floor(GlobalItemVector.size() / 3);
	this->udpsocket = new QUdpSocket(this);
	this->sender = sender;
	this->receiver = receiver;
	udpsocket->bind(QHostAddress::LocalHost, port);
	this->broadcast_ = new BroadcastThread(200, index);
	connect(udpsocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
	this->consensusGap = ConsensusGap;
	
}
void Consensus::setConsensusGap(unsigned gap)
{
	this->consensusGap = gap;
}
void Consensus::setPort(uint16_t port)
{
	this->port = port;
}

bool Consensus::ConsensusTime()
{
	Util* util = new Util();
	time_t timestamp = util->getTimeStamp();
	if (timestamp%ConsensusGap<=1000)
	{
		this->consensusTimestamp = timestamp;
		return true;
	}
	return false;
}
bool Consensus::ProposeTime()
{
	Util* util = new Util();
	time_t timestamp = util->getTimeStamp();
	if (timestamp%(consensusGap+(broadcastValueGap+proposeGap+mainnodeGap+3*waitGap)*round+broadcastValueGap+waitGap)<= 1000)
	{
		return true;
	}
	return false;
}
bool Consensus::MainnodeTime()
{
	Util* util = new Util();
	time_t timestamp = util->getTimeStamp();
	if ((timestamp-consensusTimestamp)%(broadcastValueGap+mainnodeGap+proposeGap) <= 500)
	{
		return true;
	}
	return false;
}
int Consensus::MainNodeRNG()
{
	seed = seed * 1103515245 + 15132;
	return (unsigned int)(seed / 65536) % 3276815;
}
void Consensus::setMainNodeRNGSeed(int seed)
{
	this->seed = seed;
}
int Consensus::chooseMainNode()
{
	while (true)
	{
		int mainNode = MainNodeRNG()%node_index;
		std::vector<int>::iterator result = std::find(thisViewMainNodes.begin(), thisViewMainNodes.end(), mainNode);
		if (result == thisViewMainNodes.end())
		{
			thisViewMainNodes.push_back(mainNode);
			return mainNode;
		}
	}
}



unsigned Consensus::getMajority(std::vector<ConsensusMSG> MsgVector, QString& majorString,int view, int round)
{
	std::vector<MsgCount> msgCount;
	for (int i=0;i<MsgVector.size();i++)
	{
		MsgCount msg_t = {QString::fromStdString( MsgVector[i].content),0,this->consensusView,this->round };
		std::vector<MsgCount>::iterator result = std::find(msgCount.begin(), msgCount.end(), msg_t);
		if (result==msgCount.end())
		{
			MsgCount newMsgCount = { QString::fromStdString(MsgVector[i].content),1,MsgVector[i].view ,MsgVector[i].round};
			msgCount.push_back(newMsgCount);
		}
		else
		{
			result->count++;
		}
	}
	unsigned int Max = 0;
	for (int i=0;i<msgCount.size();i++)
	{
		if (msgCount[i].count>Max)
		{
			Max = msgCount[i].count;
			majorString = msgCount[i].msg;
		}
	}
	msgCount.clear();
	return  Max;
}
void Consensus::broadcast(std::string str)
{
	Util util;
	for (int i = 0; i < GlobalItemVector.size(); i++)
	{
		int x2 = GlobalItemVector[i]->getCentre().x();
		int y2 = GlobalItemVector[i]->getCentre().y();
		int x1 = GlobalItemVector[index]->x();
		float t = util.calculateDistance(GlobalItemVector[index]->x(), GlobalItemVector[index]->y(), x2, y2);
		if (t <= GlobalItemVector[index]->getRadius())
		{
			quint16 destPort = ConsensusStartPort + GlobalItemVector[i]->getIndex();
			//Msleep((rand()*rand())%50);
			Msleep(30);
			int result = udpsocket->writeDatagram(str.c_str(), str.length(), QHostAddress::Broadcast, destPort);
		}
	}
}



void Consensus::stop()
{
	this->stopflag = true;
}
int Consensus::Routine()
{
	stopflag = false;
	srand(time(nullptr) + index);
	Util util;
	Json::Value root;
	Json::FastWriter writer;
	while (!stopflag)
	{
		Msleep(500);
		state = waiting;
		if (!ConsensusTime())
		{
			Msleep(200);
			continue;
		}
		end = util.getTimeStamp() -10*1000;
		start = 0;
		this->n = GlobalItemVector.size();
		this->f = std::floor(GlobalItemVector.size() / 3);
		consensusView++;
		recvMSG.clear();
		proposeMSG.clear();
		mainNodeMSG.clear();
		thisViewMainNodes.clear();
		round = 0;
		MerkleTreeRoot = "0000000000000000";//需要实现,需要是Json格式////////
		MerkleTreeRoot = QString::fromStdString(receiver->localDatabase->GetData(start,end));
		MerkleTreeRootT = MerkleTreeRoot;
		while (round < f + 1)
		{

			//发送线程只要管发送的事Send Thread only need to handle sending
			//获取到一定时间段内的默克尔树后就发送给周围的节点
			state = ready_to_broadcast;

			root["VIEW"] = this->consensusView;
			root["TYPE"] = 0;
			root["CONTENT"] = MerkleTreeRoot.toStdString();
			root["ROUND"] = this->round;
			root["SIGNATURE"] = this->port;
			std::string strTobeBroadcast = writer.write(root);
			//int t = (rand()*rand())%1000;
			Msleep( (rand()*rand()%300));
			Util util;
			std::string time = util.utilLocalTimeStr();
			//this->broadcast_->setSpeed(30);
			//this->broadcast_->broadcast(strTobeBroadcast);
			broadcast(strTobeBroadcast);
			QString log = tr("Consensus time: %1, broadcast value: %2\n").arg(QString::fromStdString(time)).arg(MerkleTreeRoot);
			state = propose_1;
			root.clear();

			while (!MainnodeTime())
			{
				Msleep(200);
			}
			setMainNodeRNGSeed(this->consensusView);
			thisViewMainNode = chooseMainNode();

			if (thisViewMainNode == this->index)
			{
				//If this node is the main node
				Msleep(1);//wait the gap
				root["VIEW"] = this->consensusView;
				root["TYPE"] = 2;
				root["ROUND"] = this->round;
				root["CONTENT"] = MerkleTreeRootT.toStdString();
				root["SIGNATURE"] = this->port;
				std::string str = writer.write(root);
				//broadcast(str);
				this->broadcast_->setSpeed(1);
				this->broadcast_->broadcast(str);
			}
			if (MerkleTreeRootT==MerkleTreeRoot&&round==0)
			{
				proposeFlag = false;
				receiver->localDatabase->addBlock(MerkleTreeRoot);
				receiver->localDatabase->clearDataifCorrect();
				break;
			}
			else if (proposeFlag&&round==0)
			{
				//索取正确的数据后退出
				receiver->localDatabase->addBlock(MerkleTreeRootT);
				receiver->localDatabase->clearDataifCorrect();
				proposeFlag = false;
				break;
				
			}
			else //如果到这一步，说明大部分节点的初始值都不同
			{
				verifyFlag = true;
			}
			
			round++;
			Msleep(1000);
		}
		if (verifyFlag)
		{
			Msleep(10 * 1000);
			//验证是否正确,需要进行二值共识，如果二值共识值为1则将这个值加入区块链，然后判断有哪些没上链
			//否则回滚
			verifyFlag = false;
		}
		
	}
	
	return 0;
}

void Consensus::readPendingDatagrams()
{
	Json::Value root;
	Json::FastWriter writer;
	Json::Reader reader;
	while (udpsocket->hasPendingDatagrams())
	{
		char buf[1024] = { 0 };
		QHostAddress addr;
		quint16 port;
		qint64 len;
		QString recv_addr;
		QString recv_port;
		QString recv_time;
		QString msgString = "";
		len = udpsocket->readDatagram(buf, sizeof(buf), &addr, &port);
		if (len < 0)
		{
			QString msg = "Received Wrong Data, Error code:0x00000001.\n";
			receiver->AddLog(msg);
		}
		recv_addr = addr.toString();
		recv_port = QString::number(port);
		if (!reader.parse(buf,root,false))
		{
			QString msg = "Received bad data. Cannot parse!\n";
			receiver->AddLog(msg);
		}
		std::string signature = root["SIGNATURE"].asString();//signature is represented by port number
		//if (port!= std::stoi(signature))
		//{
		//	QString msg = tr("Received bad data.Signature not valid,Signature=%1, port=%2\n").arg(QString::fromStdString(signature)).arg(recv_port);
		//	receiver->AddLog(msg);
		//}
		int src = port%10000;
		QPointF a = GlobalItemVector[src]->getCentre();
		switch (root["TYPE"].asInt())
		{
		default:
			break;
		case 0:
			
			//emit toSceneDrawArrow(2, nullptr, nullptr, a, GlobalItemVector[index]->getCentre(), a, a,Qt::red,index);
			if (src!=index)
			{
				this->receiver->toSceneDrawArrow(a, GlobalItemVector[index]->getCentre(), Qt::red, index);
			}
			if (root["VIEW"].asInt()==this->consensusView)
			{
				std::string content = root["CONTENT"].asString();
				ConsensusMSG consensusMsg;
				consensusMsg.type = 0;
				consensusMsg.content = content;
				consensusMsg.signature = signature;
				consensusMsg.uploader = src;
				consensusMsg.view = this->consensusView;
				consensusMsg.round = root["ROUND"].asInt();
				recvMSG.push_back(consensusMsg);
				Util util;
				QString time = QString::fromStdString(util.utilLocalTimeStr());
				QString log = tr("%1 :Received value from %2 :%3, view:%4,round:%5\n").arg(time).arg(src).arg(QString::fromStdString(content)).arg(this->consensusView).arg(root["ROUND"].asInt());
				receiver->AddLog(log);
			}
			break;
		case 1:
			//emit toSceneDrawArrow(2, nullptr, nullptr, a, GlobalItemVector[index]->getCentre(), a, a, Qt::darkGreen, index);
			if (src != index)
			{
				this->receiver->toSceneDrawArrow(a, GlobalItemVector[index]->getCentre(), Qt::green, index);
			}
			if (root["VIEW"].asInt() == this->consensusView)
			{
				std::string content = root["CONTENT"].asString();
				ConsensusMSG consensusMsg;
				consensusMsg.type = 1;
				consensusMsg.content = content;
				consensusMsg.signature = signature;
				consensusMsg.uploader = src;
				consensusMsg.view = this->consensusView;
				consensusMsg.round = root["ROUND"].asInt();
				proposeMSG.push_back(consensusMsg);
				Util util;
				QString time = QString::fromStdString(util.utilLocalTimeStr());
				QString log = tr("%1 :Received propose from %2 :%3,view:%4,round:%5\n").arg(time).arg(src).arg(QString::fromStdString(content)).arg(this->consensusView).arg(root["ROUND"].asInt());
				receiver->AddLog(log);
			}
			break;
		case 2:
			//emit toSceneDrawArrow(2, nullptr, nullptr, a, GlobalItemVector[index]->getCentre(), a, a, Qt::darkYellow, index);
			if (src != index)
			{
				QColor color(128, 0, 128);
				this->receiver->toSceneDrawArrow(a, GlobalItemVector[index]->getCentre(), color, index);
			}
			if (root["VIEW"].asInt() == this->consensusView)
			{
				std::string content = root["CONTENT"].asString();
				ConsensusMSG consensusMsg;
				consensusMsg.type = 2;
				consensusMsg.content = content;
				consensusMsg.signature = signature;
				consensusMsg.uploader = src;
				consensusMsg.view = this->consensusView;
				consensusMsg.round = root["ROUND"].asInt();
				//proposeMSG.push_back(consensusMsg);
				Util util;
				QString time = QString::fromStdString(util.utilLocalTimeStr());
				QString log = tr("%1 :Received mainnode from %2 :%3").arg(time).arg(src).arg(QString::fromStdString(content));
				if (content!=MerkleTreeRootT.toStdString())
				{
					mainnodeValue = content;
					if (recv_port!=thisViewMainNode)
					{
						QString msg = tr("Received bad data.Mainnode not valid,thisViewMainNode=%1,sender=%2\n").arg(thisViewMainNode).arg(recv_port);
						receiver->AddLog(msg);
						break;
					}
					//如果来自临时主节点的值不同于之前的临时MerkleTreeRoot,并且满足改变条件那么改变值
					//首先取出众数，然后判断是否满足n-f的条件
					QString majorString = "";
					unsigned int Max = getMajority(proposeMSG, majorString,this->consensusView,this->round);
					if (Max<n-f)
					{
						MerkleTreeRootT = QString::fromStdString(content);
						log.append(", change value\n");
					}
					else
					{
						log.append(", value not changed\n");
					}
					
				}
				receiver->AddLog(log);
				
			}
		
		}
		if (recvMSG.size()>=n-f)
		{
			QString majorString = "";
			unsigned int majorCount = getMajority(recvMSG, majorString, this->consensusView, this->round);
			if (majorCount>=n-f&&state==propose_1)
			{

				root["VIEW"] = this->consensusView;
				root["TYPE"] = 1;
				root["ROUND"] = this->round;
				root["CONTENT"] = majorString.toStdString();
				root["SIGNATURE"] = this->port;
				std::string str = writer.write(root);
				QString log= tr("received more than n-f value(%1), broadcast propose(%1)\n").arg(majorString);
				this->receiver->AddLog(log);
				//broadcast(str);
				this->broadcast_->setSpeed(25);
				this->broadcast_->broadcast(str);
				state = propose_2;
				proposeFlag = true;
			}
		}
		if (proposeMSG.size()>=f&&state==propose_2)
		{
			
			QString majorString = "";
			unsigned int majorCount = getMajority(proposeMSG, majorString, this->consensusView, this->round);
			if (majorCount>=f)
			{
				state = change_propose;
				if (majorString==MerkleTreeRootT)
				{
					QString log = tr("received more than f propose(%1), value not change (same) \n").arg(MerkleTreeRootT);
					this->receiver->AddLog(log);
					
				}
				else
				{
					MerkleTreeRootT = majorString;
					QString log = tr("received more than f propose(%1), value changed \n");
					this->receiver->AddLog(log);
				}
				
			}
		}
		
	}
}

