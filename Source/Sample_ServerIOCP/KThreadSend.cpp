#include "KThreadSend.h"
#include "KServer.h"
bool KThreadSend::Run()
{
	KServer* pServer = (KServer*)m_pObject;
	SOCKET sock = pServer->m_Net.m_ListenSocket;
	while (1)
	{
		EnterCriticalSection(&m_cs);
		std::list<KNetworkUser*>::iterator userIter;
		for (userIter = pServer->m_UserList.begin();
			userIter != pServer->m_UserList.end();)
		{
			int iRet = pServer->Broadcast(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->m_UserList.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		LeaveCriticalSection(&m_cs);
		Sleep(1);
	}
	return true;
}

KThreadSend::KThreadSend()
{
}

KThreadSend::KThreadSend(LPVOID pValue) : KThread(pValue)
{
}

KThreadSend::~KThreadSend()
{
}
