#include "pch.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(SceneState current) : m_CurrentSceneState(current)
{
	m_NextSceneState = SceneState::NONE;
	m_ModelIDs = 0;
	m_UITextureIDs = 0;

	m_Camera = std::make_unique<Object::CameraClass>();
}

Scene::SceneClass::~SceneClass()
{
	// 마우스 커서를 보이게 하고 위치를 중앙으로 되돌리기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}
