
#ifndef  _OSAPI_SEMAPHORE_H
#define _OSAPI_SEMAPHORE_H
#define _CRT_SECURE_NO_WARNINGS

class OS_Semaphore
{
public:
	OS_Semaphore(int initial_value=1);
	~OS_Semaphore();


	int Wait();
	int Wait(int ms);
	void Post();

private:
	int Init(int initial_value=1); // ´´½¨

private:
	void* m_Priv;
};


#endif

