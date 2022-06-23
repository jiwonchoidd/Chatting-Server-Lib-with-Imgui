#pragma once
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <d3d11.h>
#include <dxgi.h>
#include <assert.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXmath.h>
#include <iostream>
#include "KMath.h"
namespace wrl = Microsoft::WRL;
namespace dx = DirectX;
#pragma comment	(lib, "d3d11.lib")
#pragma comment	(lib, "dxgi.lib")
#pragma comment (lib, "D3DCompiler.lib")
#pragma comment	(lib, "ws2_32.lib")
extern float g_fSecPerFrame;
extern float g_fSecTimer;
extern HWND  g_hWnd;
extern RECT  g_rtClient;
extern ID3D11Device* g_pd3dDevice;
extern HINSTANCE g_hInstance;
template<class T>
class Singleton
{
public:
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

struct INPUT_MAP
{
	bool bUpKey;
	bool bDownKey;
	bool bLeftKey;
	bool bRightKey;

	bool bWKey;
	bool bSKey;
	bool bAKey;
	bool bDKey;
	bool bQKey;
	bool bEKey;
	bool bZKey;
	bool bCKey;

	//마우스 
	bool bLeftClick;
	bool bRightClick;
	bool bMiddleClick;

	bool bLeftHold;
	bool bRightHold;
	bool bMiddleHold;

	bool bLShift;
	bool bExit;
	bool bSpace;
	bool bEnter;

	int  iMouseValue[3];

	bool bFullScreen;
	bool bChangeFillMode;
	bool bChangePrimitive;
	bool bChangeCullMode;
	bool bChangeCameraType;
	bool bDebugRender;
};
extern INPUT_MAP g_InputData;

#pragma region wWinMain 매크로

//기본 매크로
#define WinSTART int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int nCmdShow){

//스트링화연산자를 이용한 매크로
#define WinRUN_OPT(NAME, WIDTH, HEIGHT) Sample g_Sample; g_Sample.InitWindows(hInstance,nCmdShow,L#NAME, WIDTH, HEIGHT); g_Sample.Run();}
#define WinMain_OPT(NAME, WIDTH, HEIGHT) WinSTART WinRUN_OPT(NAME, WIDTH, HEIGHT)

#pragma endregion