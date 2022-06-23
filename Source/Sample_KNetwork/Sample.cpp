
#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"
ImVec4 login_text_color = ImVec4(1.0f,1.0f,1.0f,1.0f);

LRESULT  Sample::ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_Net.Select_MsgProc(hWnd, msg, wParam, lParam);
}
bool Sample::Init()
{
	auto size = static_cast<DWORD>((sizeof(char)*24)+ 1);
	if (!GetComputerNameA(computer_name, &size))
	{
		strcat(computer_name, "NoName");
	}
    m_Camera.CreateViewMatrix(KVector3(0, 0, -5), KVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	m_Net.InitNetwork();
	//Ÿ�� ��Ʈ ������
	if(m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, ip_Address))
	{
		m_bConnect = true;
	}
    return true;
}
// ������ �Լ�
bool Sample::Frame()
{
	
	#pragma region IMGUI INTERFACE
	(m_Net.m_bConnect)? str_isConnect = "Online" : str_isConnect = "Offline";
	(m_Net.m_bConnect)? login_text_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f) :
						login_text_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	if (ImGui::Begin("Chat"))
	{
		
		ImGui::TextColored(login_text_color,str_isConnect.c_str());
		//������ �ȵǾ� ������ ���� �� �õ�
		if (!m_Net.m_bConnect)
		{
			ImGui::InputText("������ �Է�", ip_Address, sizeof(char) * 30);
			ImGui::SameLine();
			if (ImGui::Button(u8"���� ��õ�"))
			{
				if (m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, ip_Address))
				{
					m_bConnect = true;
				}
			}
		}
		else
		{
			ImGui::BeginChild(u8"ä��â", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing() - 15));
			ImGui::Text(m_chatItems);
			ImGui::EndChild();
			ImGui::Dummy(ImVec2(0.0f, 5));
			ImGui::InputText("", m_buffer, sizeof(m_buffer));
			
			ImGui::SameLine();

			if (ImGui::Button("Send") || g_InputData.bEnter)
			{
				char clear[MAX_PATH] = { 0, };
				KPacket kpacket(PACKET_CHAT_MSG);

				//�ε��� �̸� �޽���
				kpacket << 10 << computer_name << m_buffer;

				//���� ���� 0���� ������ ���۵��� �ʾ���
				if (m_Net.SendMsg(m_Net.m_Sock, kpacket.m_uPacket) < 0)
				{
					ZeroMemory(&m_chatItems, sizeof(char) * 2048);
					strcat(m_chatItems, "Error\n");
					m_Net.m_bConnect = false;
				}
				ImGui::SetKeyboardFocusHere(0);
				strcpy(m_buffer, clear);
			}
		}
	}
	ImGui::End();
#pragma endregion

	// ��Ŷ Ǯ ������� ä�� ���� üũ
	int iChatCnt = m_Net.m_User.m_lPacketPool.size();
	if (iChatCnt > 0 && m_iChatCount != iChatCnt)
	{
		m_iChatCount = iChatCnt;
		char clear[2048] = { 0, };
		strcpy(m_chatItems, clear);

		std::list<KPacket>::iterator iter;
		//��ȭ���� ���ѷ��� ������ ������
		if (m_Net.m_User.m_lPacketPool.size() > 10)
		{
			m_Net.m_User.m_lPacketPool.pop_front();
		}
		for (iter = m_Net.m_User.m_lPacketPool.begin();
			iter != m_Net.m_User.m_lPacketPool.end();
			iter++)
		{
			
			KChatting recv_data;
			ZeroMemory(&recv_data, sizeof(recv_data));
			(*iter) >> recv_data.index >> recv_data.name
					>> recv_data.message;
			//strcat(m_chatItems, (char*)(*iter).m_uPacket.ph.time);
			strcat(m_chatItems, recv_data.name);
			strcat(m_chatItems, " : ");
			strcat(m_chatItems, recv_data.message);
			strcat(m_chatItems, "\n");
		
			(*iter).Reset();
		}
	}
    return true;
}
bool Sample::Render()
{
    DrawTestBox(g_fSecTimer * m_Speed,
        0.0f, 0.0f, 0.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);

    return true;
}
bool Sample::Release()
{
    m_Camera.Release();
	m_Net.CloseNetwork();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(ä��â Ŭ���̾�Ʈ, 800, 600);