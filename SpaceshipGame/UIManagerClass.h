#pragma once

namespace System { class InputClass; }
namespace UI { class IUIClass; }

namespace UI
{
	class UIManagerClass
	{
	public:
		UIManagerClass();
		virtual ~UIManagerClass();

		UINT LoadUIs(Scene::SceneState scene_state);
		void Release();

		inline UINT GetTextureIDs() const { return m_UITextureIDs; }

	private:
		static bool IsInitialize;
		std::vector<std::unique_ptr<IUIClass>> m_UIs;
		UINT m_UITextureIDs = 0;

	public:
		UIManagerClass(const UIManagerClass& other) = delete;
		UIManagerClass(UIManagerClass&& other) = delete;
		UIManagerClass& operator=(const UIManagerClass& other) = delete;
		UIManagerClass& operator=(UIManagerClass&& other) = delete;
	};
}