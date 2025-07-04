#include "pch.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(SceneState current) : m_CurrentSceneState(current), m_NextSceneState(SceneState::NONE), m_IsSceneEnded(false), m_IsShowingCursor(true)
{
	AffineInfo info = { DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f),DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f), DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f) };
	m_Camera = std::make_unique<Object::CameraClass>(info);
}

Scene::SceneClass::~SceneClass()
{
	// 마우스 커서를 보이게 하고 위치를 중앙으로 되돌리기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}
