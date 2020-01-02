
#include "Mutex.h"

#ifndef _WIN32
//#if 1

#include <pthread.h>

struct OS_Mutex_Priv
{
	pthread_mutex_t hMutex;
};

OS_Mutex::OS_Mutex()
:m_Priv(NULL)
{
	Init();
}

OS_Mutex::~OS_Mutex()
{
	if(m_Priv) 
	{
		OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
		pthread_mutex_destroy(&priv->hMutex);
		delete priv;
	}
}

int OS_Mutex::Init()
{
	OS_Mutex_Priv* priv = new OS_Mutex_Priv;
	if(!priv) return -1;
	m_Priv = priv;

	// ´´½¨mutex
	if(pthread_mutex_init(&priv->hMutex, NULL) < 0)
	{
		delete priv;
		m_Priv = NULL;
		return -1;
	}

	return 0;
}

int OS_Mutex::Lock()
{
	OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
	if(!priv) return -1;

	if( pthread_mutex_lock(&priv->hMutex) < 0)
	{
		return -1;
	}

	return 0;
}

int OS_Mutex::TryLock()
{
	OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
	if(!priv) return -1;

	if(pthread_mutex_trylock(&priv->hMutex) < 0)
	{
		return -1;
	}

	return 0;
}

void OS_Mutex::Unlock()
{
	OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
	if(!priv ) return;

	pthread_mutex_unlock(&priv->hMutex);
}



#endif  // ! _WIN32


