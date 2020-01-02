
#ifndef _OSAPI_MUTEX_H
#define _OSAPI_MUTEX_H
#define _CRT_SECURE_NO_WARNINGS

class OS_Mutex
{
public:
	OS_Mutex();
	~OS_Mutex();


	int Lock();
	int TryLock();
	void Unlock();

private:
	int Init(); // ´´½¨

private:
	void* m_Priv;

};



#endif
