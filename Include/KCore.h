#pragma once
#include "KWindow.h"
#include "KDevice.h"
#include "KTimer.h"
#include "KInput.h"
#include "KSound.h"
#include "KWrite.h"
#include "KState.h"
#include "KCamera.h"
#include "ImGuiManager.h"
class KCore : public KWindow
{
public:
	float			m_Speed=1.0f;
	KTimer			m_Timer;
	KSound			m_Sound;
	KWrite			m_Write;
	KDebugCamera	m_Camera;
	ImGuiManager	m_ImGuiManager;
	bool		m_bDebugText = false;
private:
	bool	GameInit()	override;
	bool	GameRun()	override;
	bool	GameFrame();
	bool	GameRender();
	bool	GameRelease()override;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	PreRender();
	virtual bool	Render();
	virtual bool	PostRender();
	virtual bool	Release();
};

