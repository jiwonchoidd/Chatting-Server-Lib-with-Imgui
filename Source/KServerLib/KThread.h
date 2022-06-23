#pragma once
#include "KServerObj.h"
#include <process.h>
class KThread : public KServerObj
{
public:
	uintptr_t		m_hThread;
	unsigned int	m_iId;
	bool			m_bisStarted;
	LPVOID			m_pObject;
public:
	void CreateKThread();
	void CreateKThread(LPVOID pValue);
	//쓰레드가 끝날때까지 기다려라 명시.
	void Join();
	void Detach();
	virtual bool Run();
	static unsigned int WINAPI Runner(LPVOID param);
public:
	KThread();
	KThread(LPVOID pValue);
	virtual ~KThread();
};

