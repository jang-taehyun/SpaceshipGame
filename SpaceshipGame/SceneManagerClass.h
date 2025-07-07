#pragma once

namespace System { class InputClass; }
namespace Text { class TextManagerClass; }
namespace UI { class UIManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace Object
{
	class ObjectManagerClass;
	class IObjectClass;
}
namespace Scene
{
	class ISceneClass;
	class SceneFactoryClass;
}

namespace Scene
{
	class SceneManagerClass
	{
	public:
		SceneManagerClass();
		~SceneManagerClass();

	public:
		bool Frame(const System::InputClass* input, float frame_time);

		inline Text::TextManagerClass* GetTextManager() const { return m_TextManager.get(); }
		inline UI::UIManagerClass* GetUIManager() const { return m_UIManager.get(); }
		inline Object::ObjectManagerClass* GetObjectManager() { return m_ObjectManager.get(); }
		inline Sound::SoundManagerClass* GetSoundManager() const { return m_SoundManager.get(); }
		inline Object::IObjectClass* GetCamera();

	private:
		void ChangeScene();

	private:
		static bool IsInitialize;
		std::unique_ptr<ISceneClass> m_Scene = nullptr;
		std::unique_ptr<SceneFactoryClass> m_Loader = nullptr;

		std::unique_ptr<Text::TextManagerClass> m_TextManager = nullptr;
		std::unique_ptr<UI::UIManagerClass> m_UIManager = nullptr;
		std::unique_ptr<Sound::SoundManagerClass> m_SoundManager = nullptr;
		std::unique_ptr<Object::ObjectManagerClass> m_ObjectManager = nullptr;

	public:
		SceneManagerClass(const SceneManagerClass& other) = delete;
		SceneManagerClass(SceneManagerClass&& other) = delete;
		SceneManagerClass& operator=(const SceneManagerClass& other) = delete;
		SceneManagerClass& operator=(SceneManagerClass&& other) = delete;
	};

}