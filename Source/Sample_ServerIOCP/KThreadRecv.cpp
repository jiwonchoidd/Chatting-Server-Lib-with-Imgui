#include "KThreadRecv.h"
#include "KServer.h"
bool KThreadRecv::Run()
{
	KServer* pServer = (KServer*)m_pObject;
	SOCKET sock = pServer->m_Net.m_ListenSocket;
	//������� ��ȣ�� ���� ������ �Ǿ��.  Join�� ����..
	//�̺�Ʈ�� �־�� Join�� ������. ���ѷ����̿��� �Ұ���
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
