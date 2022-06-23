#pragma once
#include "KProtocol.h"
// data<<header<<index<<name<<damage<<message
// data>>header>>index>>name>>damage>>message
class KPacket
{
public :
	UPACKET m_uPacket;
	char*	m_pOffset;
public:
	char* c_str()
	{
		return m_pOffset;
	}
	void PutData(const char* pData, int iSize);
	void GetData(const char* pData, int iSize);
	void Reset();
public:
	KPacket& operator << (int data);
	KPacket& operator << (long data);
	KPacket& operator << (short data);
	KPacket& operator << (float data);
	KPacket& operator << (byte data);
	KPacket& operator << (char* data);
	KPacket& operator << (DWORD data);
	KPacket& operator << (std::string data);
public:
	KPacket& operator >> (int& data);
	KPacket& operator >> (long& data);
	KPacket& operator >> (short& data);
	KPacket& operator >> (float& data);
	KPacket& operator >> (byte& data);
	KPacket& operator >> (char* data);
	KPacket& operator >> (DWORD& data);
	KPacket& operator >> (std::string& data);
public:
	KPacket();
	KPacket(WORD iType);
	KPacket(const KPacket& packet);
	~KPacket();
};

