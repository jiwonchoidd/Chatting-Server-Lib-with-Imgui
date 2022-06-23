#include "KNetworkUser.h"
#include "KServer.h"
int KNetworkUser::DispatchRead(char* sRecvBuffer, int iRecvByte)
{
	//m_szRecvBuffer 2048�� ������ �޸� ��ġ �ٲ�, �ʱ�ȭ
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}

	memcpy(&m_szRecvBuffer[m_iWritePos], sRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;// ���ۿ� ������ ����� ��ġ
	m_iReadPos += iRecvByte; // ��Ŷ���� ��ġ�κ��� ���� ����Ʈ
	
	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// ��Ŷ �ؼ� ����
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// ��� 1���� ��Ŷ�� �����ߴٸ�
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				KPacket kPacket(pPacket->ph.type);
				memcpy(&kPacket.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);
				m_lPacketPool.push_back(kPacket);

				// ������Ŷ ó��
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);

		}
	}
	return 1;
}


int KNetworkUser::Dispatch(DWORD dwTransfer, KOV* ov)
{
	//�۾�
	delete ov;
	if (this->m_bConnect == false)
	{
		return 0;
	}
	if (ov->type == KOV::MODE_RECV)
	{
		if (!DispatchRecv(m_szRecv,dwTransfer))
		{
		}
		Recv();
	}
	if (ov->type == KOV::MODE_SEND)
	{
		//�۾����� �ƴ϶�� ų�̺�Ʈ �ߵ�
		if (!DispatchSend(dwTransfer))
		{
			
		}
	}
	return 1;
}
//������ �߼��Ѵ�. 

int KNetworkUser::DispatchRecv(char* szRecvBuffer, int iRecvByte)
{
	//m_szRecvBuffer 2048�� ������ �޸� ��ġ �ٲ�, �ʱ�ȭ
	if (m_iWritePos + iRecvByte >= 2048)
	{
		if (m_iReadPos > 0)
		{
			memmove(&m_szRecvBuffer[0], &m_szRecvBuffer[m_iPacketPos], m_iReadPos);
		}
		m_iPacketPos = 0;
		m_iWritePos = m_iReadPos;
	}

	memcpy(&m_szRecvBuffer[m_iWritePos], szRecvBuffer, iRecvByte);
	m_iWritePos += iRecvByte;// ���ۿ� ������ ����� ��ġ
	m_iReadPos += iRecvByte; // ��Ŷ���� ��ġ�κ��� ���� ����Ʈ

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// ��Ŷ �ؼ� ����
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// ��� 1���� ��Ŷ�� �����ߴٸ�
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				KPacket kPacket(pPacket->ph.type);
				memcpy(&kPacket.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);

				//ä�� �޼����� ���� ��ŶǮ�� �ְ� 
				//�ƴϸ� ���� ��ŶǮ�� ����
				if (pPacket->ph.type == PACKET_CHAT_MSG)
				{
					m_lPacketPool.push_back(kPacket);
					m_pServer->m_lPacketPool.push_back(kPacket);
				}
				else
				{
					m_lPacketPool.push_back(kPacket);
					m_pServer->m_lPacketPool.push_back(kPacket);
				}

				// ������Ŷ ó��
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);

			// ���� �ؽ�Ʈ ������ ���
			KChatting recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			*(m_lPacketPool.begin()) >> recvdata.index
				>> recvdata.name >> recvdata.message;
			
			std::wcout << recvdata.name << L" Dispatch Recv" << std::endl;
		}
	}
	return 1;
}

int KNetworkUser::DispatchSend(DWORD dwTransfer)
{
	return 0;
}

void KNetworkUser::Set(SOCKET sock, SOCKADDR_IN addr, KServer* pServer)
{
	m_pServer = pServer;
	m_bConnect = true;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;

	m_Sock = sock;
	m_Addr = addr;
	char ip[INET_ADDRSTRLEN];
	//��Ʈ��ũ �ּ� ��ȯ �Լ� �򿣵��(32bit) ->  
	m_csName = inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
	//������� �޸� ���ӵ� ����� �迭�ϴ� ������� 
	// Network Byte ������ To Host�� Byte ������ �ٲ�
	m_iPort = ntohs(addr.sin_port);
}

int KNetworkUser::Recv()
{
	// �񵿱� �ε� Readfile, writefile�̶� �Ȱ��� WSARecv
	//��Ŷ�� ��Ƽ� �ѹ��� �������� ����
	//���� �����ʹ� m_szRecv�� ����
	KOV* ov = new KOV(KOV::MODE_RECV);
	DWORD	dwRead;
	DWORD	lpflag=0;
	m_WsaRecvBuffer.len = sizeof(char)*256;
	m_WsaRecvBuffer.buf = m_szRecv;
	//�񵿱� ó���ϱ� io pending ����ó���������
	BOOL ret = WSARecv(m_Sock, &m_WsaRecvBuffer,
		1, &dwRead, &lpflag,
		(WSAOVERLAPPED*)ov, nullptr);
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return 0;
}

int KNetworkUser::SendMsg(char* msg, WORD type)
{
	KOV* ov = new KOV(KOV::MODE_SEND);
	//��Ŷ�� ���� ��������
	UPACKET uPacket;
	uPacket.ph.len = strlen(msg)+PACKET_HEADER_SIZE;
	uPacket.ph.type= type;

	DWORD	dwRead;
	DWORD	lpflag = 0;
	m_WsaSendBuffer.len = uPacket.ph.len;
	m_WsaSendBuffer.buf = (char*)&uPacket;

	BOOL ret = WSASend(m_Sock, &m_WsaRecvBuffer,
		1, &dwRead, lpflag,
		(WSAOVERLAPPED*)ov, nullptr);
	//IOPending ó�� 
	if (ret == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}
	return 0;
}

bool KNetworkUser::Disconnect()
{
	closesocket(m_Sock);
	return false;
}

KNetworkUser::KNetworkUser()
{
	
}

KNetworkUser::~KNetworkUser()
{
}
