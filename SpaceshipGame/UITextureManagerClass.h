#pragma once

namespace Graphic
{
	namespace Texture { class TextureClass; }
}

namespace Graphic
{
	namespace Texture
	{
		class UITextureManagerClass
		{
		public:
			UITextureManagerClass();
			virtual ~UITextureManagerClass();

			ID3D11ShaderResourceView* GetTexture(UITextureID id) const;
			void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT UITextureMask);

		private:
			static bool IsInitialize;

			std::map<UITextureID, std::unique_ptr<TextureClass>> m_TextureList;
			UINT m_CurrentUITextureMask = 0;

		public:
			UITextureManagerClass(const UITextureManagerClass& other) = delete;
			UITextureManagerClass(UITextureManagerClass&& other) = delete;
			UITextureManagerClass& operator=(const UITextureManagerClass& other) = delete;
			UITextureManagerClass& operator=(UITextureManagerClass&& other) = delete;
		};
	}
}