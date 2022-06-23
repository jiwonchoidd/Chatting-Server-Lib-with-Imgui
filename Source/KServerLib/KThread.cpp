#include "KThread.h"

void KThread::CreateKThread()
{
	//시작했으면 중복방지를 위해 리턴
	if (m_bisStarted) return;
	m_hThread = _beginthreadex(nullptr, 0,
				Runner, this, 0,&m_iId);
	m_bisStarted = true;
}

void KThread::CreateKThread(LPVOID pValue)
{
	if (m_bisStarted != false) return;
	m_hThread = _beginthreadex(
		nullptr, 0,
		Runner,
		this,
		0,
		&m_iId);
	m_pObject = pValue;
	m_bisStarted = true;
}
//메인쓰레드를 기다리는 기능이다. 
void KThread::Join()
{
	//메인스레드의 킬이벤트를 기다릴수도있음
	//쓰레드가 끝날때까지 기다려라 명시.
	::WaitForSingleObject((HANDLE)m_hThread, INFINITE);
}
//핸들이 없어지면 사용자가 관리 못함
void KThread::Detach()
{
	CloseHandle((HANDLE)m_hThread);
	m_hThread = 0;
}

bool KThread::Run()
{
	return false;
}

unsigned int __stdcall KThread::Runner(LPVOID param)
{
	KThread* pThread = (KThread*)param;
	//주소가 유효하지 않으면 리턴
	if (pThread != nullptr)
	{
		pThread->Run();
		return 1;
	}
	return 0;
}

KThread::KThread()
{
	m_bisStarted = false;
	m_pObject = nullptr;
	m_hThread = 0;
	m_iId = 0;
}

KThread::KThread(LPVOID pValue)
{
	m_bisStarted = false;
	m_pObject = nullptr;
	m_hThread = 0;
	m_iId = 0;
	CreateKThread(pValue);
}

KThread::~KThread()
{
	if(m_hThread!=0)
	CloseHandle((HANDLE)m_hThread);
}
