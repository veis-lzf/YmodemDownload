#pragma once
class CThread
{
public:
	CThread(void);
	virtual ~CThread(void);
	virtual void SetThreadData(DWORD dwParam = 0);
	virtual DWORD GetThreadData(void);
	void StartThread(void);
	void StopThread(void);
	// 线程任务纯虚函数，必须重载
	virtual void runTask(void) = 0;

protected:
	static DWORD ThreadProc(LPVOID lpParam); // 线程回调函数

protected:
	HANDLE m_hThread; // 线程句柄
	BOOL m_bExit; // 判断线程是否退出
	DWORD m_dwParam; // 用于外部传参使用
	DWORD m_dwThreadId;
};

