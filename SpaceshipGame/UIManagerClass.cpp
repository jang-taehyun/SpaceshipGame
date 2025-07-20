#include "pch.h"
#include "IUIClass.h"
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

UINT UI::UIManagerClass::LoadUI(ID UIID, Graphic::Texture::UITextureID TextureID)
{
	// instance 생성
	std::unique_ptr<IUIClass> ui = std::move(m_Loader->Load(UIID, TextureID));
	
	// 필요한 UI Texture ID 업데이트
	m_UITextureMask |= (1 << (static_cast<UINT>(TextureID)));

	// vector에 저장
	m_UIList.push_back(std::move(ui));

	return static_cast<UINT>(m_UIList.size() - 1);
}

void UI::UIManagerClass::Release()
{
	m_UIList.clear();
	m_UITextureMask = 0;
}

void UI::UIManagerClass::Frame(const System::InputClass* input)
{
	std::vector<std::unique_ptr<IUIClass>>::iterator iter;

	for (iter = m_UIList.begin(); iter != m_UIList.end(); ++iter)
		iter->get()->Update(input);
}

UI::IUIClass* UI::UIManagerClass::GetUI(UINT idx) const
{
	assert(idx < m_UIList.size());
	return m_UIList[idx].get();
}
