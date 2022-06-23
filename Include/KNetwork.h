#pragma once
#include "KNetworkUser.h"
class KNetwork
{
public:
	SOCKET					m_Sock;
	std::list<KNetworkUser> m_userlist;
	KNetworkUser			m_User;
public:
	bool	InitNetwork();
	bool	CloseNetwork();
	bool	InitServer(int protocol, int iport, const char* ip = nullptr);
public:
	int SendMsg(SOCKET sock, char* msg, WORD type);
	int SendMsg(SOCKET sock, UPACKET& packet);
	int AddUser(SOCKET sock);
	int RecvUser(KNetworkUser& user);
};

