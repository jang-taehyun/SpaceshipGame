#include "pch.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(SceneState current, SceneState next) : m_CurrentSceneState(current), m_NextSceneState(next), m_IsSceneEnded(false), m_IsShowingCursor(true) {}

Scene::SceneClass::~SceneClass()
{
	// 마우스 커서를 보이게 하고 위치를 중앙으로 되돌리기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}
