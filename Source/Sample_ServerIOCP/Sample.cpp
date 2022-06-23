#include "KLobbyServer.h"
#include "KObjectPool.h"
#pragma region legacy
////DWORD WINAPI RecvThread(LPVOID lparam)
//{
//	KServer* server = (KServer*)lparam;
//	SOCKET sock = server->m_Net.m_ListenSocket;
//	while (1)
//	{
//		//스레드가 특정 시그널이 발생할때까지 기다림
//		WaitForSingleObject(server->m_hMutex, INFINITE);
//
//		std::list<KNetworkUser>::iterator iter;
//		for (iter = server->m_UserList.begin();
//			iter != server->m_UserList.end();)
//		{
//			int iRet = server->m_Net.RecvUser(*iter);
//			//0보다 작거나 같음, 받아온 데이터가 없거나, 에러일 경우 
//			if (iRet <= 0)
//			{
//				iter = server->m_UserList.erase(iter);
//			}
//			else
//			{
//				iter++;
//			}
//		}
//		//운영체제한테 제어권 넘겨줌
//		ReleaseMutex(server->m_hMutex);
//		//의도적으로 Context Switching
//		Sleep(1);
//	}
//}
//DWORD WINAPI SendThread(LPVOID lparam)
//{
//	KServer* pServer = (KServer*)lparam;
//	SOCKET sock = pServer->m_Net.m_ListenSocket;
//	while (1)
//	{
//		//스레드가 특정 시그널이 발생할때까지 기다림
//		WaitForSingleObject(pServer->m_hMutex, INFINITE);
//
//		//임계구역 논시그널 상태가 되어 다른 뮤텍스는 기다림
//		std::list<KNetworkUser>::iterator iter;
//		for (iter = pServer->m_UserList.begin();
//			iter != pServer->m_UserList.end();)
//		{
//			//전체에게 보내줌
//			int iRet = pServer->Broadcast(*iter);
//			//0보다 작거나 같음, 받아온 데이터가 없거나, 에러일 경우 
//			if (iRet <= 0)
//			{
//				iter = pServer->m_UserList.erase(iter);
//			}
//			else
//			{
//				iter++;
//			}
//		}
//		//운영체제한테 제어권 넘겨줌
//		ReleaseMutex(pServer->m_hMutex);
//		//의도적으로 Context Switching
//		Sleep(1);
//	}
//}
#pragma endregion
int main()
{
	setlocale(LC_ALL, ""); // Korean해도 되지만 공백은 시스템 지역설정으로 따르게 한다.
	KLobbyServer server;
	server.Init(10000);
	server.Run();
	server.Release();

	//종료될때까지 기다려라..
	//런이 리턴이되면 종료가 됨
	//어떤 작업이 필요할때는 Join 
	//디텍치하기전에 조인해야함
	//tRecv.Join();
	//tSend.Join();

	std::cout << "Server Close" << std::endl;
	return 0;
}