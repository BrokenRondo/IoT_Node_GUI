
#include "IoT_Node_GUI.h"
#include <QtWidgets/QApplication>
#include<QgraphicsRectItem>
#include<QGraphicsScene>
#include<QGraphicsview>
#include <QThread>
#include <QTimer>
#include"actThread.h"
#include "myitem.h"
#include "myline.h"
#include "IoT_Network_Scene.h"
#include "InteractiveView.h"
#include "util.h"
#include <queue>

std::vector<MyItem*> GlobalItemVector;
std::vector<MyLine*> GlobalLineVector;
std::vector<TextItem*> GlobalTextVector;
std::vector<QString> GlobalLogVector;
std::queue<QTimer*>GlobalTimerVector;
const int ConsensusStartPort = 60000;
const int DataNetworkStartPort = 50000;
time_t start;
time_t end;
unsigned int ConsensusGap;
unsigned int node_index;
int fieldSize;
int rangeSize;
int main(int argc, char *argv[])
{
	Util util;
	node_index = 0;
	fieldSize = 1000;
	rangeSize = 3000;
	ConsensusGap = 12 * 1000;
	start = 0;
	end = 0;
	end = util.getTimeStamp()-2000;
	QApplication a(argc, argv);


	IoT_Node_GUI w;
	w.show();
	return a.exec();
}
