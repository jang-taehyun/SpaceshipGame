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

		void LoadUIs(Scene::SceneState scene_state);
		void Update(const System::InputClass* input) const;

	private:
		void ReleaseUIs();

	private:
		static bool IsInitialize;
		std::vector<std::unique_ptr<IUIClass>> m_UIs;

	public:
		UIManagerClass(const UIManagerClass& other) = delete;
		UIManagerClass(UIManagerClass&& other) = delete;
		UIManagerClass& operator=(const UIManagerClass& other) = delete;
		UIManagerClass& operator=(UIManagerClass&& other) = delete;
	};
}