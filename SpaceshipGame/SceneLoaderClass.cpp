#include "pch.h"
#include "StartSceneClass.h"
#include "SceneLoaderClass.h"

bool Scene::SceneLoaderClass::IsInitailize = false;

std::unique_ptr<Scene::ISceneClass> Scene::CreateStartScene(SceneState cur, SceneState next)
{
	return std::make_unique<StartSceneClass>(cur, next);
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

std::unique_ptr<Scene::ISceneClass> Scene::SceneLoaderClass::CreateScene(SceneState cur, SceneState next)
{
	std::map<SceneState, std::function<std::unique_ptr<ISceneClass>(SceneState, SceneState)>>::iterator iter;

	iter = m_Creator.find(cur);
	assert(m_Creator.end() != iter);

	return iter->second(cur, next);
}
