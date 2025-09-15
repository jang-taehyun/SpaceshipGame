#include "pch.h"
#include "StartSceneClass.h"
#include "ModeChooseSceneClass.h"
#include "SingleModeSceneClass.h"
#include "SceneFactoryClass.h"

bool Scene::SceneFactoryClass::IsInitailize = false;

Scene::SceneFactoryClass::SceneFactoryClass()
{
	assert(!IsInitailize);
	IsInitailize = true;

	m_Creator.insert(std::make_pair(ID::START, CreateStartScene));
	m_Creator.insert(std::make_pair(ID::MODE_CHOOSE, CreateModeChooseScene));
	m_Creator.insert(std::make_pair(ID::SINGLE_MODE, CreateSingleModeScene));
}

Scene::SceneFactoryClass::~SceneFactoryClass()
{
	IsInitailize = false;
}

std::unique_ptr<Scene::ISceneClass> Scene::SceneFactoryClass::CreateScene(
	ID current,
	Object::ObjectManagerClass* actors,
	Text::TextManagerClass* texts,
	UI::UIManagerClass* UIs,
	Sound::SoundManagerClass* sounds
)
{
	std::map<ID, std::function<std::unique_ptr<ISceneClass>(
		Object::ObjectManagerClass*,
		Text::TextManagerClass*,
		UI::UIManagerClass*,
		Sound::SoundManagerClass*)>>::iterator iter;

	iter = m_Creator.find(current);
	assert(m_Creator.end() != iter);

	return iter->second(actors, texts, UIs, sounds);
}

std::unique_ptr<Scene::ISceneClass> Scene::CreateStartScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	// 다음 scene 지정
	ID next = ID::MODE_CHOOSE;

	// instance 생성
	std::unique_ptr<Scene::ISceneClass> scene = std::make_unique<StartSceneClass>(next, actors, texts, UIs, sounds);
	assert(scene);

	return scene;
}

std::unique_ptr<Scene::ISceneClass> Scene::CreateModeChooseScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	// 다음 scene 지정
	ID next = ID::SINGLE_MODE;

	// instance 생성
	std::unique_ptr<Scene::ISceneClass> scene = std::make_unique<ModeChooseSceneClass>(next, actors, texts, UIs, sounds);
	assert(scene);

	return scene;
}

std::unique_ptr<Scene::ISceneClass> Scene::CreateSingleModeScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	// 다음 scene 지정
	ID next = ID::MODE_CHOOSE;

	// instance 생성
	std::unique_ptr<Scene::ISceneClass> scene = std::make_unique<SingleModeSceneClass>(next, actors, texts, UIs, sounds);
	assert(scene);

	return scene;
}
