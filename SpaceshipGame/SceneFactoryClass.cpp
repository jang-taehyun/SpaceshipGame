#include "pch.h"
#include "StartSceneClass.h"
#include "ModeChooseSceneClass.h"
#include "SceneFactoryClass.h"

bool Scene::SceneFactoryClass::IsInitailize = false;

Scene::SceneFactoryClass::SceneFactoryClass()
{
	assert(!IsInitailize);
	IsInitailize = true;

	m_Creator.insert(std::make_pair(SceneID::START, CreateStartScene));
	m_Creator.insert(std::make_pair(SceneID::MODE_CHOOSE, CreateModeChooseScene));
}

Scene::SceneFactoryClass::~SceneFactoryClass()
{
	IsInitailize = false;
}

std::unique_ptr<Scene::ISceneClass> Scene::SceneFactoryClass::CreateScene(SceneID current, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	std::map<SceneID, std::function<std::unique_ptr<ISceneClass>(Object::ObjectManagerClass*, Text::TextManagerClass*, UI::UIManagerClass*, Sound::SoundManagerClass*)>>::iterator iter;

	iter = m_Creator.find(current);
	assert(m_Creator.end() != iter);

	return iter->second(actors, texts, UIs, sounds);
}

std::unique_ptr<Scene::ISceneClass> Scene::CreateStartScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	// 다음 scene 지정
	SceneID next = SceneID::MODE_CHOOSE;

	// instance 생성
	std::unique_ptr<Scene::ISceneClass> scene = std::make_unique<StartSceneClass>(next, actors, texts, UIs, sounds);
	assert(scene);

	return scene;
}

std::unique_ptr<Scene::ISceneClass> Scene::CreateModeChooseScene(Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
{
	// 다음 scene 지정
	SceneID next = SceneID::START;

	// instance 생성
	std::unique_ptr<Scene::ISceneClass> scene = std::make_unique<ModeChooseSceneClass>(next, actors, texts, UIs, sounds);
	assert(scene);

	return scene;
}