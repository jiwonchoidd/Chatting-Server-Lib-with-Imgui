#include "KServer.h"

//���� ��ŶǮ ����� ������� ��ü �������� ������. ��ε�ĳ��Ʈ
int KServer::BroadcastUserPacket(KNetworkUser* user)
{
	if (user->m_lPacketPool.size() > 0)
	{
		std::list<KPacket>::iterator iter;
		for (iter = user->m_lPacketPool.begin();
			iter != user->m_lPacketPool.end();)
		{
			for (KNetworkUser* senduser : m_UserList)
			{
				int iRet = m_Net.SendMsg(senduser->m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser->m_bConnect = false;
				}
			}
			iter = user->m_lPacketPool.erase(iter);
		}
	}
	return 1;
}

bool KServer::AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr)
{
	return true;
}

bool KServer::DelUser(SOCKET clientSock)
{
	return true;
}

bool KServer::DelUser(KNetworkUser* pUser)
{
	pUser->Disconnect();
	return true;
}

//������ ��ɿ� �°Բ�.
bool KServer::Init(int port)
{
	//���� ���� �ʱ�ȭ
	if (!m_Net.InitNetwork())
	{
		return false;
	}
	if (!m_Net.InitServer(SOCK_STREAM, port, nullptr))
	{
		return false;
	}

	std::cout << "\n::::::::::::::::Server Start.::::::::::::::::\n" << std::endl;
	std::cout << "-> type '/help' to get help\n" << std::endl;

	u_long on = 1;
	ioctlsocket(m_Net.m_ListenSocket, FIONBIO, &on);

	//���� �޾Ƶ帮�� ������ ������
	m_Acceptor.CreateKThread(this);
	m_Acceptor.Detach();
	m_Commander.CreateKThread(this);
	m_Commander.Detach();

	return true;
}

bool KServer::Run()
{

	return true;
}

bool KServer::Release()
{
	KObjectPool<KNetworkUser>::AllFree();
	KObjectPool<KOV>::AllFree();

	closesocket(m_Net.m_ListenSocket);
	WSACleanup();

	return true;
}

KServer::KServer()
{
}

KServer::~KServer()
{
}
