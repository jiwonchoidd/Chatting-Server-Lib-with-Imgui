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
	//�����尡 ���������� ��ٷ��� ���.
	void Join();
	void Detach();
	virtual bool Run();
	static unsigned int WINAPI Runner(LPVOID param);
public:
	KThread();
	KThread(LPVOID pValue);
	virtual ~KThread();
};

