#pragma once

#include <functional>

namespace Scene { class ISceneClass; }

namespace Scene
{
	std::unique_ptr<ISceneClass> CreateStartScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);

	class SceneLoaderClass
	{
	public:
		SceneLoaderClass();
		virtual ~SceneLoaderClass();

		std::unique_ptr<ISceneClass> CreateScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);

	private:
		static bool IsInitailize;
		std::map<SceneState, std::function<std::unique_ptr<ISceneClass>(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)>> m_Creator;
	};
}
