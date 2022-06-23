#include "KAsyncSelect.h"
//재정의 윈도우 메세지 프로시저
LRESULT  KAsyncSelect::Select_MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case NETWORK_MSG:
	{
		WORD wRet = WSAGETSELECTEVENT(lParam);
		switch (wRet)
		{
		case FD_CONNECT:
		{
			//커넥트 시도
		}break;
		case FD_CLOSE:
		{
			m_bConnect = false;
		}break;
		case FD_READ:
		{
			RecvUser(m_User);
		}break;
		case FD_WRITE:
		{
			m_bConnect = true;
			//m_bConnect = false;
		}break;
		}
	}break;
	}
	return 0;
}
bool KAsyncSelect::Connect(HWND hWnd, int type, int iport, const char* ip)
{
	//소켓 구조체 채우기
	m_Sock = socket(AF_INET, type, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iport);
	sa.sin_addr.s_addr = inet_addr(ip);
	m_User.m_Sock = m_Sock;

	//윈도우 메세지를 통해서 비동기적으로 소켓 활용 가능
	if (WSAAsyncSelect(m_Sock, hWnd, NETWORK_MSG,
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		return false;
	}
	// 블록킹 연결시도 오류가 발생하지 않으면 0을 반환,  다른 소켓 어플리케이션에 연결
	int iRet = WSAConnect(m_Sock, (sockaddr*)&sa, sizeof(sa), NULL, NULL, NULL, NULL);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}