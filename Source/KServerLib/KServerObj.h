#pragma once
#include "KNetstd.h"
class KServerObj
{
	public:
		CRITICAL_SECTION m_cs;
		//Join�� ���� �̺�Ʈ ����
		HANDLE			 m_hKillEvent;
	public:
		KServerObj();
		virtual ~KServerObj();
};

