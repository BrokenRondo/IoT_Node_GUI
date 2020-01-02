
#ifndef _OSAPI_THREAD_H
#define _OSAPI_THREAD_H
#define _CRT_SECURE_NO_WARNINGS
class OS_Thread
{
public:
	OS_Thread();
	virtual ~OS_Thread();

	// 创建并启动
	virtual int Run();

	// 等待和收回资源
	static void Join(OS_Thread* thrd);

	// Sleep函数
	static void Msleep(int ms);
	static void Sleep(int s);

public:
	virtual int Routine() = 0;

private:
	void* m_Priv;
};


#endif

