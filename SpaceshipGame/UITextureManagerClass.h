#pragma once

#include <SpriteBatch.h>

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

		void LoadUITexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT TextureIDs);

	private:
		void ReleaseUITexture();

	private:
		static bool IsInitialize;
		std::map<Graphic::Texture::UITextureID, std::unique_ptr<Graphic::Texture::TextureClass>> m_TextureList;

	public:
		UITextureManagerClass(const UITextureManagerClass& other) = delete;
		UITextureManagerClass(UITextureManagerClass&& other) = delete;
		UITextureManagerClass& operator=(const UITextureManagerClass& other) = delete;
		UITextureManagerClass& operator=(UITextureManagerClass&& other) = delete;
	};
}