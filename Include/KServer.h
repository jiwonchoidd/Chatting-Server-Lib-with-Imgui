#pragma once
#include "KNetwork.h"
#include "KServerObj.h"
#include "KAcceptor.h"
#include "KCommandServer.h"
//브로드캐스트는 서버에서 처리해줘야함
class KServer : public KServerObj
{
	KAcceptor				 m_Acceptor;
	KCommandServer			 m_Commander;
public:
	std::list<KNetworkUser*> m_UserList;
	KNetwork				 m_Net;
	std::list<KPacket>		 m_lPacketPool;
	typedef std::list<KNetworkUser*>::iterator m_UserIter;
public:
	int BroadcastUserPacket(KNetworkUser* user);
	virtual bool AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr);
protected:
	virtual bool DelUser(SOCKET clientSock);
	virtual bool DelUser(KNetworkUser* pUser);
public:
	virtual	bool		Init(int port);
	virtual	bool		Run();
	virtual	bool		Release();

public:
	KServer();
	virtual ~KServer();
};

