#include "pch.h"
#include "IUIClass.h"
#include "UIManagerClass.h"

bool UI::UIManagerClass::IsInitialize = false;
static ErrorContent e;

UI::UIManagerClass::UIManagerClass()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("UIManagerClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 UIManagerClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

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

void UI::UIManagerClass::Update(bool IsLeftMouseButtonPressed) const
{
	for (int i = 0; i < m_UIs.size(); ++i)
		m_UIs[i]->Update(IsLeftMouseButtonPressed);
}

void UI::UIManagerClass::ReleaseUIs()
{
	m_UIs.clear();
}
