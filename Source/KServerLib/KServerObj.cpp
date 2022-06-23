#include "KServerObj.h"

KServerObj::KServerObj()
{
	m_hKillEvent = CreateEvent(0, TRUE, FALSE, NULL);;
	InitializeCriticalSection(&m_cs);
}

KServerObj::~KServerObj()
{
	DeleteCriticalSection(&m_cs);
	CloseHandle(m_hKillEvent);
}
