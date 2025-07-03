#include "pch.h"
#include "IUIClass.h"
#include "UIManagerClass.h"

bool UI::UIManagerClass::IsInitialize = false;

UI::UIManagerClass::UIManagerClass()
{
	assert(!IsInitialize);
	IsInitialize = true;
}

UI::UIManagerClass::~UIManagerClass()
{
	Release();
	IsInitialize = false;
}

UINT UI::UIManagerClass::LoadUIs(Scene::SceneState scene_state)
{

}

void UI::UIManagerClass::Release()
{
	m_UIs.clear();
	m_UITextureIDs = 0;
}
