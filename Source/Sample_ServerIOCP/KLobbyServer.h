#pragma once
#include "KServer.h"
#include "KChatUser.h"
#define MAX_WORKER_THREAD 3


class KLobbyServer : public KServer
{
public:
	HANDLE			m_hWorkThread[MAX_WORKER_THREAD];
	/*완료포트 관리 객체*/
	HANDLE			m_hIOCP;

public:
	virtual	bool		Init(int port);
	virtual	bool		Run();
	virtual	bool		Release();
	virtual bool		AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr);
};

