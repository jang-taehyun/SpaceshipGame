#pragma once

#include <functional>

namespace Scene { class ISceneClass; }

namespace Scene
{
	std::unique_ptr<ISceneClass> CreateStartScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);

	class SceneFactoryClass
	{
	public:
		SceneFactoryClass();
		virtual ~SceneFactoryClass();

		std::unique_ptr<ISceneClass> CreateScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);

	private:
		static bool IsInitailize;
		std::map<SceneState, std::function<std::unique_ptr<ISceneClass>(SceneState, Object::ObjectManagerClass*, Text::TextManagerClass*, UI::UIManagerClass*, Sound::SoundManagerClass*)>> m_Creator;
	};
}
