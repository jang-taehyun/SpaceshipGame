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
		private:
			const UINT m_UITextureIDCount = 6;
			std::map<UITextureID, const std::wstring> m_UITextureFileList =
			{
				{ UITextureID::LOGO, _T("./resource/logo.png")},
				{ UITextureID::START_BACKGROUND, _T("./resource/StartBackground.jpg") },
				{ UITextureID::START_BUTTON, _T("./resource/button-icon-png-21058.png") },
				{ UITextureID::ECS_POPUP_BACKGROUND, _T("./resource/Popup-Background.png")},
				{ UITextureID::ECS_POPUP, _T("./resource/PopUp.png")},
			};

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