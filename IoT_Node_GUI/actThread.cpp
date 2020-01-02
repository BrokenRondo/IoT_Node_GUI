#include "actThread.h"
#include <QPointF>
#include "myitem.h"
#include "myline.h"
#include <QMutex>
#include <queue>
#include <mutex>
extern std::vector<MyItem*> GlobalItemVector;
extern std::vector<MyLine*> GlobalLineVector;
extern std::vector<TextItem*> GlobalTextVector;
extern std::vector<QString> GlobalLogVector;
extern std::queue<QTimer*>GlobalTimerVector;

//This thread is used to clean timer which are already ended(not active)
void actThread::run()
{
	std::mutex mutex;
	this->stop = false;
	while (!stop) {
		msleep(100);
		while (!GlobalTimerVector.empty())
		{
			if (GlobalTimerVector.front()==nullptr)
			{
				GlobalTimerVector.pop();
			}
			if (!(GlobalTimerVector.front()->isActive()))
			{
				try
				{
					mutex.lock();
					GlobalTimerVector.front()->stop();
					delete GlobalTimerVector.front();
					GlobalTimerVector.front() = nullptr;
					GlobalTimerVector.pop();
					msleep(1);
					mutex.unlock();
					//usleep(1000);
				}
				catch (...)
				{

					continue;
				}
				
			}
			else
			{
				msleep(100);
			}
			
		}
	

	}
}

void actThread::setMainView(QGraphicsView *view)
{
	this->mainView = view;
}
void actThread::setStop(bool stop)
{
	this->stop = true;
}

