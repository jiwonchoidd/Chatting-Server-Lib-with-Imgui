#pragma once
#include "KServerObj.h"
#include "KPacket.h"
#include "KObjectPool.h"
class KServer;
//�񵿱� �۾��� �Ϸ�������� OVERLAPPED ���� �Ǿ� �־����.
//
struct KOV : public KObjectPool<KOV>
{
	//��ܿ� ������ ĳ������ �����ϴ�.
	enum{MODE_RECV =1, MODE_SEND =2, MODE_EXIT= 3};
	OVERLAPPED ov;
	int type;

	/*������Ʈ Ǯ ������� ������ �Ǵµ�, 
	�����ڿ��� Zeromemory�� �̰� ������ */
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
//�ٸ� ������ �������� ����� ������ �ȵȴ�. 
//���� ������ ���������� ��ӹ޾Ƽ� ���
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
	// buffer ����� �ʿ���
	char			m_szRecvBuffer[2048];
	int				m_iPacketPos; // ��Ŷ�� ���� �ּ�
	int				m_iWritePos; // ������ ���� ��ġ 
	int				m_iReadPos; // ��Ŷ ���� ��ġ�� ���� ���� ����Ʈ
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

