#include "pch.h"
#include "StartSceneClass.h"
#include "SceneLoaderClass.h"

bool Scene::SceneLoaderClass::IsInitailize = false;

std::unique_ptr<Scene::ISceneClass> Scene::CreateStartScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	return std::make_unique<StartSceneClass>(current, actors, texts, UIs, sounds);
}

Scene::SceneLoaderClass::SceneLoaderClass()
{
	m_Creator.insert(std::make_pair(SceneState::START, CreateStartScene));

	IsInitailize = true;
}

Scene::SceneLoaderClass::~SceneLoaderClass()
{
	IsInitailize = false;
}

std::unique_ptr<Scene::ISceneClass> Scene::SceneLoaderClass::CreateScene(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	std::map<SceneState, std::function<std::unique_ptr<ISceneClass>(SceneState current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)>>::iterator iter;

	iter = m_Creator.find(current);
	assert(m_Creator.end() != iter);

	return iter->second(current, actors, texts, UIs, sounds);
}
