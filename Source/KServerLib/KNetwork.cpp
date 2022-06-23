#include "KNetwork.h"

//���� ���α׷� ���� 2.2
bool KNetwork::InitNetwork()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	return true;
}

bool KNetwork::InitServer(int protocol, int iport, const char* ip)
{
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
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
		sa.sin_addr.s_addr = inet_pton(AF_INET, ip, &sa);
	}

	//���Ͽ� �ּ� �Ҵ�
	int iRet = bind(m_ListenSocket, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return false;

	//Ŭ���̾�Ʈ�� ���� ���� ��û�� ó���Ҽ��ִ� ���¸� ���� 
	iRet = listen(m_ListenSocket, SOMAXCONN);
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
		//WSASend IOCP�������� �����ؾ���
			iSendByte= send(sock, &pMsg[iSize],
			packet.ph.len - iSendByte, 0);

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
		//WSASend IOCP�������� �����ؾ���
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

//����� �������κ��� �����͸� ������. 
//�����ϸ� 1
int KNetwork::RecvUser(KNetworkUser* user)
{
	char szRecvBuffer[1024] = { 0, };
	//recv �ޱ�
	int iRecvByte = recv(user->m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			return -1;
		}
		return 2;
	}
	user->DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}
KNetwork::KNetwork()
{
	m_ListenSocket=NULL;
	m_User = nullptr;
}

bool KNetwork::CloseNetwork()
{
	closesocket(m_ListenSocket);
	WSACleanup();
	return true;
}
KNetwork::~KNetwork()
{
}
