#pragma once

namespace System { class InputClass; }
namespace UI
{
	class IUIClass;
	class UIFactoryClass;
}

namespace UI
{
	class UIManagerClass
	{
	public:
		UIManagerClass();
		virtual ~UIManagerClass();

		IUIClass* LoadBackground(Graphic::Texture::UITextureID TextureID);
		IUIClass* LoadUI(ID UIID, Graphic::Texture::UITextureID TextureID);
		void Release();

		inline UINT GetUITextureMask() const { return m_UITextureMask; }

		IUIClass* GetUI(int idx) const;
		IUIClass* GetBackground() const { return m_Background.get(); }

		inline UINT GetUICount() const { return static_cast<UINT>(m_UIList.size()); }

	private:
		static bool IsInitialize;

		std::vector<std::unique_ptr<IUIClass>> m_UIList;
		std::unique_ptr<IUIClass> m_Background = nullptr;
		UINT m_UITextureMask = 0;

		std::unique_ptr<UIFactoryClass> m_Loader = nullptr;

	public:
		UIManagerClass(const UIManagerClass& other) = delete;
		UIManagerClass(UIManagerClass&& other) = delete;
		UIManagerClass& operator=(const UIManagerClass& other) = delete;
		UIManagerClass& operator=(UIManagerClass&& other) = delete;
	};
}