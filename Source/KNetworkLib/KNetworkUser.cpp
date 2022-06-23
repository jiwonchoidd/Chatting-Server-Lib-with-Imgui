#include "KNetworkUser.h"

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

void KNetworkUser::Set(SOCKET sock, SOCKADDR_IN addr)
{
	m_bConnect = true;
	ZeroMemory(m_szRecvBuffer, sizeof(char) * 2048);
	m_iPacketPos = 0;
	m_iWritePos = 0;
	m_iReadPos = 0;

	m_Sock = sock;
	m_Addr = addr;
	//네트워크 주소 변환 함수 빅엔디안(32bit) ->  
	m_csName = inet_ntoa(addr.sin_addr);
	//엔디안은 메모리 연속된 대상을 배열하는 방법으로 
	// Network Byte 순서를 To Host의 Byte 순서로 바꿈
	m_iPort = ntohs(addr.sin_port);
}
