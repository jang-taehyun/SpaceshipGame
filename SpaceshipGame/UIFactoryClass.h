#pragma once

#include <functional>

namespace UI { class IUIClass; }

namespace UI
{
	std::unique_ptr<IUIClass> CreateDefault(Graphic::Texture::UITextureID TextureID);

	class UIFactoryClass
	{
	public:
		UIFactoryClass();
		~UIFactoryClass();

		std::unique_ptr<IUIClass> Load(ID UIID, Graphic::Texture::UITextureID TextureID) const;

	private:
		static bool IsInitialize;

		std::map<ID, std::function<std::unique_ptr<IUIClass>(Graphic::Texture::UITextureID ID)>> m_Creator;
	};
}


