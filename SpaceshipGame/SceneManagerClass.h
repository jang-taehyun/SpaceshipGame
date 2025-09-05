#pragma once

#include "ISceneClass.h"

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

		Text::TextManagerClass* GetTextManager() const { return m_TextManager.get(); }
		UI::UIManagerClass* GetUIManager() const { return m_UIManager.get(); }
		Object::ObjectManagerClass* GetObjectManager() { return m_ObjectManager.get(); }
		Sound::SoundManagerClass* GetSoundManager() const { return m_SoundManager.get(); }

		Graphic::Terrain::TerrainID GetTerrainID() const { return m_Scene->GetTerrainID(); }
		Graphic::Terrain::SkyDomeID GetSkyDomeID() const { return m_Scene->GetSkyDomeID(); }

	private:
		void Release();
		void ChangeScene(ID next);

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