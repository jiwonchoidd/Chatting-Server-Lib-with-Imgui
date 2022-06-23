#include "KAcceptor.h"
#include "KServer.h"
bool KAcceptor::Run()
{
	KServer* pServer = (KServer*)m_pObject;
	SOCKET sock = pServer->m_Net.m_ListenSocket;
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	while (1)
	{

		//지정된 킬이벤트가 신호를 받았다면 break
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}

		SOCKET clientSock = accept(pServer->m_Net.m_ListenSocket,
			(sockaddr*)&clientAddr, &iLen);
		if (clientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << "ErrorCode=" << iError << std::endl;
				break;
			}
		}
		//유저가 들어왔음
		else
		{
			//서버에서 유저 추가한다.
			pServer->AddUser(clientSock, clientAddr);
			//Acceptor는 받아드리는 작업만 한다.
			char ip[INET_ADDRSTRLEN];
			std::cout
				<< "" << std::endl
				<< "IP : " << inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN) << "\t"
				<< "PORT : " << ntohs(clientAddr.sin_port) << std::endl;
			std::cout << "Current : " << pServer->m_UserList.size() << " 명 접속중.." << std::endl;

		}
		Sleep(1);
	}
	//setevent
	return true;
}

KAcceptor::KAcceptor()
{
}

KAcceptor::KAcceptor(LPVOID pValue) : KThread(pValue)
{
}

KAcceptor::~KAcceptor()
{
}
