#pragma once
#include "KNetworkUser.h"
class KNetwork
{
public:
	SOCKET						m_ListenSocket;
	std::list<KNetworkUser*>	m_Userlist;
	KNetworkUser*				m_User;
public:
	bool	InitNetwork();
	bool	CloseNetwork();
	bool	InitServer(int protocol, int iport, const char* ip = nullptr);
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int RecvUser(KNetworkUser* user);

	KNetwork();
	~KNetwork();
};

