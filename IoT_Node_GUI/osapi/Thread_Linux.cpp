
#ifndef _WIN32
//#if 1
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "Thread.h"


struct OS_Thread_Priv
{
	pthread_t hThread;
};

OS_Thread::OS_Thread() 
: m_Priv(NULL)
{
}

OS_Thread::~OS_Thread()
{
	if(m_Priv)
	{
		OS_Thread_Priv* priv = (OS_Thread_Priv*) m_Priv;
		delete priv;
	}
}

static void* OS_Thread_Proc_Linux(void* param)
{
	OS_Thread* thrd = (OS_Thread*) param;
	thrd->Routine();
	return NULL;
}

int OS_Thread::Run()
{
	// 创建私有结构
	OS_Thread_Priv* priv = new OS_Thread_Priv;
	if(!priv) return -1;

	m_Priv = priv;

	// 创建线程
	if(pthread_create(&priv->hThread, NULL, OS_Thread_Proc_Linux, this) < 0)
	{
		delete priv;
		m_Priv = NULL;
		return -1;
	}

	return 0;
}

void OS_Thread::Join(OS_Thread* thrd)
{
	OS_Thread_Priv* priv = (OS_Thread_Priv*) thrd->m_Priv;
	if(priv)
	{
		pthread_join(priv->hThread, NULL);

		// 删除资源
		delete priv;
		thrd->m_Priv = NULL;
	}
}

void OS_Thread::Msleep(int ms)
{
	//::usleep(ms * 1000);
	// 好像使用nanosleep更好

	timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

void OS_Thread::Sleep(int s)
{
	::sleep(s);
}

int OS_Thread::Routine()
{
	return 0;
}




#endif // ! _WIN32


