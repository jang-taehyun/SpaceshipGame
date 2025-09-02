#include "pch.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(ID current, ID next)
	: m_CurrentSceneID(current),
	m_NextSceneID(next)
{}

Scene::SceneClass::~SceneClass()
{
	SetShowMouseCursor(true);
}

void Scene::SceneClass::SetShowMouseCursor(bool IsShow)
{
	CURSORINFO cursorInfo = {};
	cursorInfo.cbSize = sizeof(cursorInfo);

	if (GetCursorInfo(&cursorInfo))
	{
		// 현재 마우스 커서가 보이지 않는다면 보이게 하기 //
		if (!cursorInfo.flags && IsShow)
			ShowCursor(true);

		// 현재 마우스 커서가 보인다면 안 보이게 하기 //
		else if (cursorInfo.flags && !IsShow)
		{
			while (cursorInfo.flags)
			{
				ShowCursor(false);
				GetCursorInfo(&cursorInfo);
			}
		}
	}
	else
		assert(false);
}
