#include "pch.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(ID current, ID next)
	: m_CurrentSceneState(current),
	m_NextSceneState(next)
{}

Scene::SceneClass::~SceneClass()
{
	CURSORINFO cursorInfo = {};
	cursorInfo.cbSize = sizeof(cursorInfo);

	// 현재 마우스 커서가 보이지 않는다면 보이게 하기 //
	if (GetCursorInfo(&cursorInfo))
	{
		if (!cursorInfo.flags)
			ShowCursor(true);
	}
	else
		assert(false);
}
