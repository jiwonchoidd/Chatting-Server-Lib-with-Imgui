#include "KThreadRecv.h"
#include "KServer.h"
bool KThreadRecv::Run()
{
	KServer* pServer = (KServer*)m_pObject;
	SOCKET sock = pServer->m_Net.m_ListenSocket;
	//죽으라는 신호를 보내 리턴이 되어야.  Join이 가능..
	//이벤트가 있어야 Join이 가능함. 무한루프이여서 불가능
	while (1)
	{
		/*if (WaitForSingleObject(pServer->m_hKillEvent, 1))
		{
			break;
		}
		*/

		WaitForSingleObject(pServer->m_hMutex, INFINITE);

		std::list<KNetworkUser*>::iterator userIter;
		for (userIter = pServer->m_UserList.begin();
			userIter != pServer->m_UserList.end();)
		{
			int iRet = pServer->m_Net.RecvUser(*userIter);
			if (iRet <= 0)
			{
				userIter = pServer->m_UserList.erase(userIter);
			}
			else
			{
				userIter++;
			}
		}
		ReleaseMutex(pServer->m_hMutex);
		Sleep(1);
	}
	return true;
}

KThreadRecv::KThreadRecv()
{
}

KThreadRecv::KThreadRecv(LPVOID pValue) : KThread(pValue)
{
}

KThreadRecv::~KThreadRecv()
{
}
