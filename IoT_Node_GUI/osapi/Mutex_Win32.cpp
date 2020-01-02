
#include "Mutex.h"

#ifdef _WIN32
#include <windows.h>

struct OS_Mutex_Priv
{
	HANDLE hMutex;
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
		delete priv;
	}
}

int OS_Mutex::Init()
{
	OS_Mutex_Priv* priv = new OS_Mutex_Priv;
	if(!priv) return -1;
	m_Priv = priv;

	priv->hMutex = CreateMutex(NULL, true, NULL);
	if(priv->hMutex == NULL)
	{
		delete priv;
		m_Priv = NULL;
		return -1;
	}

	ReleaseMutex(priv->hMutex);
	return 0;
}

int OS_Mutex::Lock()
{
	OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
	if(!priv) return -1;

	WaitForSingleObject(priv->hMutex, INFINITE);
	return 0;
}

int OS_Mutex::TryLock()
{
	OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
	if(!priv) return -1;

	DWORD  ret = WaitForSingleObject(priv->hMutex, 1);	
	if( ret == WAIT_OBJECT_0)
	{
		return 0; // success
	}
	if( ret == WAIT_TIMEOUT)
	{
		return -1; // timeout
	}
	return -1;
}

void OS_Mutex::Unlock()
{
	OS_Mutex_Priv* priv = (OS_Mutex_Priv*) m_Priv;
	if(!priv) return;

	ReleaseMutex(priv->hMutex);
}

#endif





