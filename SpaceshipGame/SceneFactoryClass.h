#pragma once

#include <functional>

namespace Scene { class ISceneClass; }

namespace Scene
{
	std::unique_ptr<ISceneClass> CreateStartScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);
	std::unique_ptr<ISceneClass> CreateModeChooseScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);

	class SceneFactoryClass
	{
	public:
		SceneFactoryClass();
		virtual ~SceneFactoryClass();

		std::unique_ptr<ISceneClass> CreateScene(SceneID current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds);

	private:
		static bool IsInitailize;
		std::map<SceneID, std::function<std::unique_ptr<ISceneClass>(Object::ObjectManagerClass*, Text::TextManagerClass*, UI::UIManagerClass*, Sound::SoundManagerClass*)>> m_Creator;
	};
}
