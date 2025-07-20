#pragma once

#include <functional>

namespace UI { class IUIClass; }

namespace UI
{
	std::unique_ptr<IUIClass> CreateDefault(ID UIID, Graphic::Texture::UITextureID TextureID);
	std::unique_ptr<IUIClass> CreateButton(ID UIID, Graphic::Texture::UITextureID TextureID);

	class UIFactoryClass
	{
	public:
		UIFactoryClass();
		~UIFactoryClass();

		std::unique_ptr<IUIClass> Load(ID UIID, Graphic::Texture::UITextureID TextureID) const;

	private:
		static bool IsInitialize;

		std::map<ID, std::function<std::unique_ptr<IUIClass>(ID, Graphic::Texture::UITextureID)>> m_Creator;
	};
}


