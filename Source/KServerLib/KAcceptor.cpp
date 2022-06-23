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

		//������ ų�̺�Ʈ�� ��ȣ�� �޾Ҵٸ� break
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
		//������ ������
		else
		{
			//�������� ���� �߰��Ѵ�.
			pServer->AddUser(clientSock, clientAddr);
			//Acceptor�� �޾Ƶ帮�� �۾��� �Ѵ�.
			char ip[INET_ADDRSTRLEN];
			std::cout
				<< "" << std::endl
				<< "IP : " << inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN) << "\t"
				<< "PORT : " << ntohs(clientAddr.sin_port) << std::endl;
			std::cout << "Current : " << pServer->m_UserList.size() << " �� ������.." << std::endl;

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
