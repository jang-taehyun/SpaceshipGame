#include "pch.h"
#include "UIClass.h"
#include "ButtonClass.h"
#include "UIFactoryClass.h"

bool UI::UIFactoryClass::IsInitialize = false;

UI::UIFactoryClass::UIFactoryClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

	m_Creator.insert(std::make_pair(ID::BACKGROUND, CreateDefault));
	m_Creator.insert(std::make_pair(ID::DEFAULT, CreateDefault));
	m_Creator.insert(std::make_pair(ID::BUTTON, CreateButton));
}

UI::UIFactoryClass::~UIFactoryClass()
{
	IsInitialize = false;
}

std::unique_ptr<UI::IUIClass> UI::UIFactoryClass::Load(ID UIID, Graphic::Texture::UITextureID TextureID) const
{
	std::map<ID, std::function<std::unique_ptr<IUIClass>(ID, Graphic::Texture::UITextureID)>>::const_iterator iter;

	iter = m_Creator.find(UIID);
	assert(m_Creator.end() != iter);

	return iter->second(UIID, TextureID);
}

std::unique_ptr<UI::IUIClass> UI::CreateDefault(ID UIID, Graphic::Texture::UITextureID TextureID)
{
	std::unique_ptr<UI::IUIClass> ui = std::make_unique<UIClass>(UIID, TextureID);
	assert(ui);

	return ui;
}

std::unique_ptr<UI::IUIClass> UI::CreateButton(ID UIID, Graphic::Texture::UITextureID TextureID)
{
	std::unique_ptr<UI::IUIClass> ui = std::make_unique<ButtonClass>(UIID, TextureID);
	assert(ui);

	return ui;
}
