#include "pch.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(ID current, ID next)
	: m_CurrentSceneState(current),
	m_NextSceneState(next)
{}

Scene::SceneClass::~SceneClass()
{
	// 마우스 커서를 보이게 하기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
}
