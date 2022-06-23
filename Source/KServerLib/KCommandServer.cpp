#include "KCommandServer.h"
#include "KServer.h"
void KCommandServer::HelpMode()
{
	std::cout << "��ɾ� ����Ʈ" << std::endl;
	for (m_iter_map = m_CommandMap.begin();
		m_iter_map != m_CommandMap.end();)
	{
		std::cout << m_iter_map->first << " \n";
		m_iter_map++;
	}
}
void KCommandServer::ExitMode()
{
	KServer* pServer = (KServer*)m_pObject;
	std::cout << "������ �����մϴ�." << std::endl;
	::SetEvent(pServer->m_hKillEvent);
	pServer->Release();
}
void KCommandServer::KickMode()
{
	std::cout << "���� �߹� �̱���." << std::endl;
}
/*Command Line Interface
 */
bool KCommandServer::Run()
{
	KServer* pServer = (KServer*)m_pObject;

	while (1)
	{
		//������ ų�̺�Ʈ�� ��ȣ�� �޾Ҵٸ� break
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}

		std::cout << "->";
		std::cin >> m_Input;
		std::cout << "\n";
		for (m_iter_map = m_CommandMap.begin();
			m_iter_map != m_CommandMap.end();)
		{
			if (m_Input == m_iter_map->first)
			{
				m_CommandMode =m_iter_map->second;
				break;
			}
			else
			{
				m_iter_map++;
				if(m_iter_map== m_CommandMap.end())
				std::cout << "�ش� ��ɾ� ���� '/help' for more information" << std::endl;
			}
		}

		m_Input = "";
		std::cin.clear();
		Sleep(2);

		if (m_CommandMode != 0)
		{
			switch (m_CommandMode)
			{
			case COMMAND_HELP:
			{
				HelpMode();
			}break;
			case COMMAND_EXIT:
			{
				ExitMode();
			}break;
			case COMMAND_KICK:
			{
				KickMode();
			}break;
			default:
				break;
			}
			m_CommandMode = 0;
		}
	}
	return true;
}

KCommandServer::KCommandServer()
{
}

KCommandServer::KCommandServer(LPVOID pValue)
{
}

KCommandServer::~KCommandServer()
{
}
