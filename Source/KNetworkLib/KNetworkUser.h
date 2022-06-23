#pragma once
#include "KPacket.h"

class KNetworkUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	bool		m_bConnect = false;
	// buffer 기능이 필요함
	char		m_szRecvBuffer[2048];
	int			m_iPacketPos; // 패킷의 시작 주소
	int			m_iWritePos; // 현재의 저장 위치 
	int			m_iReadPos; // 패킷 시작 위치로 부터 받은 바이트
	std::list<KPacket> m_lPacketPool;
	int DispatchRead(char* sRecvBuffer, int iRecvByte);
	void Set(SOCKET sock, SOCKADDR_IN addr);
};

