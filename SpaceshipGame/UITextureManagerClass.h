#pragma once

namespace Graphic
{
	namespace Texture
	{
		class TextureClass;
	}
}

namespace UI
{
	class UITextureManagerClass
	{
	public:
		UITextureManagerClass();
		virtual ~UITextureManagerClass();

		void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT UITextureMask);

	private:
		static bool IsInitialize;

		std::map<Graphic::Texture::UITextureID, std::unique_ptr<Graphic::Texture::TextureClass>> m_TextureList;
		UINT m_CurrentUITextureMask = 0;

	public:
		UITextureManagerClass(const UITextureManagerClass& other) = delete;
		UITextureManagerClass(UITextureManagerClass&& other) = delete;
		UITextureManagerClass& operator=(const UITextureManagerClass& other) = delete;
		UITextureManagerClass& operator=(UITextureManagerClass&& other) = delete;
	};
}