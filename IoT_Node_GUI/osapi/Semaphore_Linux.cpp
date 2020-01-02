
#include "Semaphore.h"

#ifndef _WIN32
//#if 1
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h> 

struct OS_Semaphore_Priv
{
	sem_t hSem;
};

OS_Semaphore::OS_Semaphore(int initial_value)
:m_Priv (NULL)
{
	Init(initial_value);
}

OS_Semaphore::~OS_Semaphore()
{
	if(m_Priv)
	{
		OS_Semaphore_Priv* priv = (OS_Semaphore_Priv*) m_Priv;
		sem_destroy(&priv->hSem);
		delete priv;
	}
}

int OS_Semaphore::Init(int initial_value)
{
	OS_Semaphore_Priv* priv = new OS_Semaphore_Priv;
	if(!priv) return -1;
	m_Priv = priv;

	if(sem_init(&priv->hSem, 1, initial_value) < 0)
	{
		delete priv;
		m_Priv = NULL;
		return -1;
	}

	return 0;
}

int OS_Semaphore::Wait()
{
	OS_Semaphore_Priv* priv = (OS_Semaphore_Priv*) m_Priv;
	if(!priv) return -1;

	if(sem_wait(&priv->hSem) < 0)
	{
		return -1;
	}

	return 0;
}

int OS_Semaphore::Wait(int ms)
{
	OS_Semaphore_Priv* priv = (OS_Semaphore_Priv*) m_Priv;
	if(!priv) return -1;

	timeval tv_now;
	gettimeofday(&tv_now, NULL);

	timespec ts;
	ts.tv_sec = tv_now.tv_sec;
	ts.tv_nsec = tv_now.tv_usec * 1000;

	int ns = ts.tv_nsec + (ms % 1000)  * 1000000;
	ts.tv_nsec = ns % 1000000000;
	ts.tv_sec += ns / 1000000000;
	ts.tv_sec += ms / 1000;

	if(sem_timedwait(&priv->hSem, &ts) != 0)
	{
		return -1;
	}

	return 0;;
}

void OS_Semaphore::Post()
{
	OS_Semaphore_Priv* priv = (OS_Semaphore_Priv*) m_Priv;
	if(!priv) return;

	sem_post(&priv->hSem);
}

#endif // ! _WIN32


