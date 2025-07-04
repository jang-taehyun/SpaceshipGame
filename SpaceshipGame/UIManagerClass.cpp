#include "pch.h"
#include "IUIClass.h"
#include "BackgroundClass.h"
#include "UIFactoryClass.h"
#include "UIManagerClass.h"

bool UI::UIManagerClass::IsInitialize = false;

UI::UIManagerClass::UIManagerClass() : m_UITextureMask(0)
{
	assert(!IsInitialize);

	m_Loader = std::make_unique<UIFactoryClass>();
	assert(m_Loader);

	IsInitialize = true;
}

UI::UIManagerClass::~UIManagerClass()
{
	Release();
	IsInitialize = false;
}

UI::IUIClass* UI::UIManagerClass::LoadBackground(Graphic::Texture::UITextureID TextureID)
{
	// instance 생성
	m_Background = std::make_unique<BackgroundClass>(TextureID);
	assert(m_Background);

	// 필요한 UI Texture ID 업데이트
	m_UITextureMask |= (1 << static_cast<UINT>(TextureID));

	return m_Background.get();
}

UI::IUIClass* UI::UIManagerClass::LoadUI(ID UIID, Graphic::Texture::UITextureID TextureID)
{
	// instance 생성
	std::unique_ptr<IUIClass> ui = std::move(m_Loader->Load(UIID, TextureID));
	
	// 필요한 UI Texture ID 업데이트
	m_UITextureMask |= (1 << (static_cast<UINT>(TextureID)));

	// map에 저장
	m_UIList.push_back(std::move(ui));

	return m_UIList[m_UIList.size() - 1].get();
}

void UI::UIManagerClass::Release()
{
	m_Background.reset();
	m_UIList.clear();
	m_UITextureMask = 0;
}

UI::IUIClass* UI::UIManagerClass::GetUI(int idx) const
{
	assert(idx < m_UIList.size());
	return m_UIList[idx].get();
}
