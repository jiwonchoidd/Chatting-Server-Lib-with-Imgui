#pragma once
#include "KNetstd.h"
class KServerObj
{
	public:
		CRITICAL_SECTION m_cs;
		//Join를 위해 이벤트 생성
		HANDLE			 m_hKillEvent;
	public:
		KServerObj();
		virtual ~KServerObj();
};

