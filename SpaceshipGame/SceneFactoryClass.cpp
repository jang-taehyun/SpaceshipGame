#include "pch.h"
#include "StartSceneClass.h"
#include "SceneFactoryClass.h"

bool Scene::SceneFactoryClass::IsInitailize = false;

std::unique_ptr<Scene::ISceneClass> Scene::CreateStartScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	std::unique_ptr<Scene::ISceneClass> scene = std::make_unique<StartSceneClass>(current, actors, texts, UIs, sounds);
	assert(scene);

	return scene;
}

Scene::SceneFactoryClass::SceneFactoryClass()
{
	m_Creator.insert(std::make_pair(SceneState::START, CreateStartScene));

	IsInitailize = true;
}

Scene::SceneFactoryClass::~SceneFactoryClass()
{
	IsInitailize = false;
}

std::unique_ptr<Scene::ISceneClass> Scene::SceneFactoryClass::CreateScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	std::map<SceneState, std::function<std::unique_ptr<ISceneClass>(SceneState, Object::ObjectManagerClass*, Text::TextManagerClass*, UI::UIManagerClass*, Sound::SoundManagerClass*)>>::iterator iter;

	iter = m_Creator.find(current);
	assert(m_Creator.end() != iter);

	return iter->second(current, actors, texts, UIs, sounds);
}
