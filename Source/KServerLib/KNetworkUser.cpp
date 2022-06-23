#include "KNetworkUser.h"
#include "KServer.h"
int KNetworkUser::DispatchRead(char* sRecvBuffer, int iRecvByte)
{
	//m_szRecvBuffer 2048을 넘으면 메모리 위치 바꿈, 초기화
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
	m_iWritePos += iRecvByte;// 버퍼에 이전에 저장된 위치
	m_iReadPos += iRecvByte; // 패킷시작 위치로부터 받은 바이트
	
	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// 적어도 1개의 패킷이 도착했다면
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				KPacket kPacket(pPacket->ph.type);
				memcpy(&kPacket.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);
				m_lPacketPool.push_back(kPacket);

				// 다음패킷 처리
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
	//작업
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
		//작업중이 아니라면 킬이벤트 발동
		if (!DispatchSend(dwTransfer))
		{
			
		}
	}
	return 1;
}
//받은걸 발송한다. 

int KNetworkUser::DispatchRecv(char* szRecvBuffer, int iRecvByte)
{
	//m_szRecvBuffer 2048을 넘으면 메모리 위치 바꿈, 초기화
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
	m_iWritePos += iRecvByte;// 버퍼에 이전에 저장된 위치
	m_iReadPos += iRecvByte; // 패킷시작 위치로부터 받은 바이트

	if (m_iReadPos >= PACKET_HEADER_SIZE)
	{
		// 패킷 해석 가능
		UPACKET* pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
		// 적어도 1개의 패킷이 도착했다면
		if (pPacket->ph.len <= m_iReadPos)
		{
			do {
				KPacket kPacket(pPacket->ph.type);
				memcpy(&kPacket.m_uPacket,
					&m_szRecvBuffer[m_iPacketPos],
					pPacket->ph.len);

				//채팅 메세지면 유저 패킷풀에 넣고 
				//아니면 서버 패킷풀에 넣음
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

				// 다음패킷 처리
				m_iPacketPos += pPacket->ph.len;
				m_iReadPos -= pPacket->ph.len;
				if (m_iReadPos < PACKET_HEADER_SIZE)
				{
					break;
				}
				pPacket = (UPACKET*)&m_szRecvBuffer[m_iPacketPos];
			} while (pPacket->ph.len <= m_iReadPos);

			// 서버 텍스트 받은거 출력
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
	//네트워크 주소 변환 함수 빅엔디안(32bit) ->  
	m_csName = inet_ntop(AF_INET, &(addr.sin_addr), ip, INET_ADDRSTRLEN);
	//엔디안은 메모리 연속된 대상을 배열하는 방법으로 
	// Network Byte 순서를 To Host의 Byte 순서로 바꿈
	m_iPort = ntohs(addr.sin_port);
}

int KNetworkUser::Recv()
{
	// 비동기 로드 Readfile, writefile이랑 똑같은 WSARecv
	//패킷을 모아서 한버에 보낼수도 있음
	//실제 데이터는 m_szRecv로 들어옴
	KOV* ov = new KOV(KOV::MODE_RECV);
	DWORD	dwRead;
	DWORD	lpflag=0;
	m_WsaRecvBuffer.len = sizeof(char)*256;
	m_WsaRecvBuffer.buf = m_szRecv;
	//비동기 처리니까 io pending 예외처리해줘야함
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
	//패킷을 만들어서 보내야함
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
	//IOPending 처리 
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
