#pragma once
#ifndef CONSENSUS
#define CONSENSUS
#include <QtNetwork/QtNetwork>
#include <QThread>
#include "SimNodeSendThread.h"
#include "SimNodeRecvThread.h"
#include "osapi/Thread.h"
#include "myitem.h"
#include "myline.h"
#include "broadcastThread.h"

class SimNodeRecvThread;
class MyItem;
struct ConsensusMSG
{
	int view = 0;
	int round = 0;
	int type = 0;
	std::string content = "";
	int uploader = 0;
	std::string signature = "";
};
class Consensus:public QObject,public OS_Thread
{
	Q_OBJECT
public:
	Consensus(unsigned int index);
	Consensus(unsigned int index,uint16_t port);
	Consensus(unsigned int index, uint16_t port, SimNodeSendThread *sender, SimNodeRecvThread *receiver);

	void stop();
	void setPort(uint16_t port);
	void setConsensusGap(unsigned int gap);
	unsigned int getMajority(std::vector<ConsensusMSG> MsgVector, QString &majorString,int view, int round);
	void broadcast(std::string);
	int MainNodeRNG();
	void setMainNodeRNGSeed(int seed);
	int chooseMainNode();
	BroadcastThread* broadcast_;
signals:
	void toSceneDrawArrow(unsigned int, MyItem*, MyLine*, QPointF, QPointF, QPointF, QPointF, QColor, unsigned int index);
	void toSceneDeleteLine(unsigned int);

	
private:
	bool ConsensusTime();
	bool ProposeTime();
	bool MainnodeTime();
	unsigned int RandomLeader();
	enum State{ waiting, recv_mn, ready_to_broadcast, recv_1, propose_1, no_propose, propose_2,propose1_propose2, change_propose, mainNode_broad,		listening_mainNode };
	State state;
	unsigned int time_m=0, time_n=0;
	bool mainNode = false;
	unsigned int recv_num = 0;
	int Routine() override;
	QUdpSocket *udpsocket;
	unsigned int index;
	bool stopflag= false;
	uint16_t port;
	unsigned int consensusGap = 20*1000;
	unsigned int broadcastValueGap = 6000;
	unsigned int proposeGap = 6000;
	unsigned int mainnodeGap = 1000;
	unsigned int waitGap = 300;
	unsigned int round = 0;
	unsigned int consensusTimestamp = 0;
	SimNodeSendThread *sender;
	SimNodeRecvThread *receiver;
	unsigned int consensusView = 0;
	QString MerkleTreeRoot = "";
	QString MerkleTreeRootT = "";
	QString MerkleProposeValue = "";
	unsigned int f;
	unsigned int n;
	std::vector<ConsensusMSG> recvMSG;
	std::vector<ConsensusMSG> proposeMSG;
	std::vector<ConsensusMSG> mainNodeMSG;
	int thisViewMainNode=0;
	std::vector<int> thisViewMainNodes;
	unsigned int seed = 0;//used for RNG
	bool proposeFlag = false;
	bool verifyFlag = false;
	std::string mainnodeValue;


private slots:
	void readPendingDatagrams();
	
};




#endif
