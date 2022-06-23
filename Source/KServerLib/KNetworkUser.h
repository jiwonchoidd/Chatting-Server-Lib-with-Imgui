#pragma once
#include "KServerObj.h"
#include "KPacket.h"
#include "KObjectPool.h"
class KServer;
//비동기 작업이 완료시점까지 OVERLAPPED 유지 되어 있어야함.
//
struct KOV : public KObjectPool<KOV>
{
	//상단에 있으면 캐스팅이 가능하다.
	enum{MODE_RECV =1, MODE_SEND =2, MODE_EXIT= 3};
	OVERLAPPED ov;
	int type;

	/*오브젝트 풀 사용으로 재사용이 되는데, 
	생성자에서 Zeromemory로 이걸 방지함 */
	KOV()
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		type = MODE_RECV;
	}
	KOV(int packetType)
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		type = packetType;
	}
};
//다른 서버에 종속적인 기능이 있으면 안된다. 
//게임 유저면 게임유저로 상속받아서 사용
class KNetworkUser : public KServerObj
{
public:
	KServer*	m_pServer;
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	bool		m_bConnect = false;

	WSABUF		m_WsaRecvBuffer;
	WSABUF		m_WsaSendBuffer;
	char		m_szRecv[2048];
	char		m_szSend[2048];
public:
	// buffer 기능이 필요함
	char			m_szRecvBuffer[2048];
	int				m_iPacketPos; // 패킷의 시작 주소
	int				m_iWritePos; // 현재의 저장 위치 
	int				m_iReadPos; // 패킷 시작 위치로 부터 받은 바이트
	std::list<KPacket> m_lPacketPool;
public:
	int		DispatchRead(char* sRecvBuffer, int iRecvByte);
	int		DispatchRecv(char* sRecvBuffer, int iRecvByte);
	int		DispatchSend(DWORD dwTransfer);
	int		Dispatch(DWORD dwTransfer, KOV* tov);
	void	Set(SOCKET sock, SOCKADDR_IN addr, KServer* pServer);
	int		Recv();
	int		SendMsg(char* msg, WORD type);
	bool	Disconnect();
public:
	KNetworkUser();
	virtual ~KNetworkUser();
};

