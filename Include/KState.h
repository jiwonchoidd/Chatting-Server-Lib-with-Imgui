#pragma once
#include "KStd.h"
//뎁스 스텐실 상태, 블렌드 상태, 레스터라이즈 상태, 샘플러 상태 관리
//수정가능한것들을 한쪽으로 몬다. 국지화라고 함
class KState
{
public:
	//DSS
	static ID3D11DepthStencilState* g_pCurrentDSS;
	static ID3D11DepthStencilState* g_pDSState;

public:
	static HRESULT		CreateDepthStenState();

public:
	static bool		SetState();
	static bool		ReleaseState();
};
static ID3D11DepthStencilState* ApplyDSS(ID3D11DeviceContext* pContext,
	ID3D11DepthStencilState* pState)
{
	ID3D11DepthStencilState* pDSS = KState::g_pCurrentDSS;
	pContext->OMSetDepthStencilState(pState, 1);
	KState::g_pCurrentDSS = pState;
	return pDSS;
}

