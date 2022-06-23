#pragma once
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h> // inet_ntop
#include <iostream>
#include <functional>
#include <list>
#include <string>
#include <map>
#pragma comment	(lib, "ws2_32.lib")

#ifdef DEBUG
#pragma comment	(lib, "KServerLib_d.lib")
#endif // DEBUG

namespace kserver_lib
{
	template<class Type>
	class KSingleton
	{
	public:
		static Type& GetInstance()
		{
			static Type theSingle;
			return theSingle;
		}
	};

	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  �ҽ�
			retData, iLength, // ���
			NULL, NULL);
		return retData;
	}
}