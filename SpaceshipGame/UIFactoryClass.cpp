#include "pch.h"
#include "UIClass.h"
#include "UIFactoryClass.h"

bool UI::UIFactoryClass::IsInitialize = false;

std::unique_ptr<UI::IUIClass> UI::CreateDefault(Graphic::Texture::UITextureID TextureID)
{
	std::unique_ptr<UI::IUIClass> ui = std::make_unique<UIClass>(TextureID);
	assert(ui);

	return ui;
}

UI::UIFactoryClass::UIFactoryClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

	m_Creator.insert(std::make_pair(ID::DEFAULT, CreateDefault));
}

UI::UIFactoryClass::~UIFactoryClass()
{
	IsInitialize = false;
}

std::unique_ptr<UI::IUIClass> UI::UIFactoryClass::Load(ID UIID, Graphic::Texture::UITextureID TextureID) const
{
	std::map<ID, std::function<std::unique_ptr<IUIClass>(Graphic::Texture::UITextureID ID)>>::const_iterator iter;

	iter = m_Creator.find(UIID);
	assert(m_Creator.end() != iter);

	return iter->second(TextureID);
}
