#include "KPacket.h"

void KPacket::PutData(const char* pData, int iSize)
{
	m_uPacket.ph.len += iSize;
	memcpy(m_pOffset, pData, iSize);
	m_pOffset += iSize;
}

void KPacket::GetData(const char* pData, int iSize)
{
	memcpy(const_cast<char*>(pData), m_pOffset, iSize);
	m_pOffset += iSize;
}

KPacket::KPacket()
{
	//패킷 길이 패킷 헤더 사이즈로 지정
	//오프셋은 메세지로 지정
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_pOffset = m_uPacket.msg;
}

KPacket::KPacket(WORD iType)
{
	//타입을 추가적으로 받아오는 생성자
	ZeroMemory(&m_uPacket, sizeof(UPACKET));
	m_uPacket.ph.len = PACKET_HEADER_SIZE;
	m_uPacket.ph.type = iType;
	m_pOffset = m_uPacket.msg;
}
//읽고 나면 리셋
void KPacket::Reset()
{
	m_pOffset = m_uPacket.msg;
}
KPacket::KPacket(const KPacket& packet)
{
	//패킷 자체를 받아오는 생성자
	m_uPacket.ph = packet.m_uPacket.ph;
	memcpy(m_uPacket.msg, packet.m_uPacket.msg, sizeof(char)*4096);
	//m_uPacket = packet.m_uPacket;
	m_pOffset = m_uPacket.msg;
}

KPacket::~KPacket()
{
}

#pragma region 연산자 재정의 << 데이터 Putdata
KPacket& KPacket::operator << (int data)
{
	PutData((char*)&data, sizeof(int));
	return *this;
}
KPacket& KPacket::operator << (long data)
{
	PutData((char*)&data, sizeof(long));
	return *this;
}
KPacket& KPacket::operator << (short data)
{
	PutData((char*)&data, sizeof(short));
	return *this;
}
KPacket& KPacket::operator << (float data)
{
	PutData((char*)&data, sizeof(float));
	return *this;
}
KPacket& KPacket::operator<<(byte data)
{
	PutData((char*)&data, sizeof(byte));
	return *this;
}
KPacket& KPacket::operator << (char* data)
{
	//문자열에는 널이 포함된다.
	//문자열의 끝에는 null 문자가 포함되어 있기 때문
	PutData(data, strlen(data) + 1);
	return *this;
}
KPacket& KPacket::operator<<(DWORD data)
{
	PutData((char*)&data, sizeof(DWORD));
	return *this;
}
KPacket& KPacket::operator << (std::string data)
{
	//널이 포함되지 않아서 +1 해준다.
	PutData(data.c_str(), data.size() + 1);
	return *this;
}
#pragma endregion

#pragma region 연산자 재정의 >> 데이터 Getdata
KPacket& KPacket::operator >> (int& data)
{
	GetData((char*)&data, sizeof(int));
	return *this;
}
KPacket& KPacket::operator >> (long& data)
{
	GetData((char*)&data, sizeof(long));
	return *this;
}
KPacket& KPacket::operator >> (short& data)
{
	GetData((char*)&data, sizeof(short));
	return *this;
}
KPacket& KPacket::operator >> (float& data)
{
	GetData((char*)&data, sizeof(float));
	return *this;
}
KPacket& KPacket::operator>>(byte& data)
{
	GetData((char*)&data, sizeof(byte));
	return *this;
}
KPacket& KPacket::operator >> (char* data)
{
	int iSize = strlen(m_pOffset) + 1;
	GetData(data, iSize);
	return *this;
}
KPacket& KPacket::operator>>(DWORD& data)
{
	GetData((char*)&data, sizeof(DWORD));
	return *this;
}
KPacket& KPacket::operator >> (std::string& data)
{
	int iSize = strlen(m_pOffset + 1);
	GetData(data.c_str(), iSize);
	return *this;
}
#pragma endregion


