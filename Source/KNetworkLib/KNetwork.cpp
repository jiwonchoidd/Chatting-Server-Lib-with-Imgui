#include "KNetwork.h"

//��ũ��ũ WSAStartup �ʱ�ȭ
bool KNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}

//������ �Լ� ���ε� ����, Ŭ���̾�Ʈ ���� �غ�
bool KNetwork::InitServer(int protocol, int iport, const char* ip)
{
	m_Sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iport);
	if (ip == nullptr)
	{
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		sa.sin_addr.s_addr = inet_addr(ip);
	}

	//���Ͽ� �ּ� �Ҵ�
	int iRet = bind(m_Sock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return false;

	//Ŭ���̾�Ʈ�� ���� ���� ��û�� ó���Ҽ��ִ� ���¸� ���� 
	iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return false;
	return true;
}

int KNetwork::SendMsg(SOCKET sock, char* msg, WORD type)
{
	//��Ŷ ����
	UPACKET	packet;
	ZeroMemory(&packet, sizeof(packet));
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE; //ph�� ������� ũ��� Ÿ���� �˷���
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	//�ü�� sendbuffer�� ��Ŷ ����
	char* pMsg = (char*)&packet;
	int iSize = 0;
	do {
		//send�Լ�
		int iSendByte = 0;
			iSendByte= send(sock, &pMsg[iSize],
			packet.ph.len - iSendByte, 0);

		//���ŷ�� �ƴϿ��� �̰� ���� �ʿ� ����
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSize += iSendByte;
	} while (iSize < packet.ph.len);
	return iSize;
}
int KNetwork::SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSize],
			packet.ph.len - iSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSize += iSendByte;
	} while (iSize < packet.ph.len);
	//ũ�⸸ŭ �������� �ݺ���
	return iSize;
}
int KNetwork::AddUser(SOCKET sock)
{
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	SOCKET clientSock = accept(sock,
		(sockaddr*)&clientAddr, &iLen);
	if (clientSock == SOCKET_ERROR)
	{
		return -1;
	}
	else
	{
		KNetworkUser user;
		user.Set(clientSock, clientAddr);
		m_userlist.push_back(user);
		std::cout
			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
			<< "port =" << ntohs(clientAddr.sin_port)
			<< "  " << std::endl;
		std::cout << m_userlist.size() << " �� ������.." << std::endl;
	}
	return 1;
}
int KNetwork::RecvUser(KNetworkUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	//recv �ޱ�
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		return -1;
	}
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}
bool KNetwork::CloseNetwork()
{
	closesocket(m_Sock);
	WSACleanup();
	return true;
}
