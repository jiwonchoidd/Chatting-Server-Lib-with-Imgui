#include "KThread.h"

void KThread::CreateKThread()
{
	//���������� �ߺ������� ���� ����
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
//���ξ����带 ��ٸ��� ����̴�. 
void KThread::Join()
{
	//���ν������� ų�̺�Ʈ�� ��ٸ���������
	//�����尡 ���������� ��ٷ��� ���.
	::WaitForSingleObject((HANDLE)m_hThread, INFINITE);
}
//�ڵ��� �������� ����ڰ� ���� ����
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
	//�ּҰ� ��ȿ���� ������ ����
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
