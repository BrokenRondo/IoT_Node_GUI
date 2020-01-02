
#ifdef _WIN32

#include "Thread.h"
#include <windows.h>
#include <process.h> 


struct OS_Thread_Priv
{
	HANDLE hThread;
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

static DWORD WINAPI OS_Thread_Proc_Win32_1(LPVOID param)
{
	OS_Thread* thrd = (OS_Thread*) param;
	thrd->Routine();

	return 0;
}

static void OS_Thread_Proc_Win32_2(void* param)
{
	OS_Thread* thrd = (OS_Thread*) param;
	thrd->Routine();
}

static unsigned int WINAPI OS_Thread_Proc_Win32_3(void* param)
{
	OS_Thread* thrd = (OS_Thread*) param;
	thrd->Routine();
	return 0;
}

int OS_Thread::Run()
{
	// 创建私有结构
	OS_Thread_Priv* priv = new OS_Thread_Priv;
	if(!priv) return -1;

	m_Priv = priv;

	// 创建线程
// 	DWORD nTheadId;
// 	priv->hThread = _beginthreadex(NULL, NULL, OS_Thread_Proc_Win32, this,  0, &nTheadId);
// 	priv->hThread = (HANDLE) _beginthread(OS_Thread_Proc_Win32, 0, this);
	unsigned int thrdaddr ;
	priv->hThread = (HANDLE) _beginthreadex(NULL, 0, OS_Thread_Proc_Win32_3, this, 0, &thrdaddr);
	if(priv->hThread == NULL)
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
		WaitForSingleObject(priv->hThread, INFINITE);
// 		CloseHandle(priv->hThread);
		// 删除资源
		delete priv;
		thrd->m_Priv = NULL;
	}
}

void OS_Thread::Msleep(int ms)
{
	::Sleep(ms);
}

void OS_Thread::Sleep(int s)
{
	::Sleep(s * 1000);
}

int OS_Thread::Routine()
{
	return 0;
}

#endif  //_WIN32


