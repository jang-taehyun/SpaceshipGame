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
	ReleaseUIs();
	IsInitialize = false;
}

void UI::UIManagerClass::LoadUIs(Scene::SceneState scene_state)
{

}

void UI::UIManagerClass::Update(const System::InputClass* input) const
{
	for (int i = 0; i < m_UIs.size(); ++i)
		m_UIs[i]->Update(input);
}

void UI::UIManagerClass::ReleaseUIs()
{
	m_UIs.clear();
}
