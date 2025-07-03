#pragma once

namespace System { class InputClass; }
namespace Text { class TextManagerClass; }
namespace UI { class UIManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace Object { class ObjectManagerClass; }
namespace Scene
{
	class ISceneClass;
	class SceneLoaderClass;
}

namespace Scene
{
	class SceneManagerClass
	{
	public:
		SceneManagerClass();
		~SceneManagerClass();

	public:
		void Frame(const System::InputClass* input, float frame_time);

		virtual inline Text::TextManagerClass* GetTextManager() const;
		virtual inline UI::UIManagerClass* GetUIManager() const;
		virtual inline Object::ObjectManagerClass* GetObjectManager() const;
		virtual inline Sound::SoundManagerClass* GetSoundManager() const;
		virtual inline Object::IObjectClass* GetCamera() const;

	private:
		void ChangeScene();

	private:
		static bool IsInitialize;
		std::unique_ptr<ISceneClass> m_Scene = nullptr;
		std::unique_ptr<SceneLoaderClass> m_SceneLoader = nullptr;

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