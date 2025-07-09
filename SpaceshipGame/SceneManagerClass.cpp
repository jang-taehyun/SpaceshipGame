#include "pch.h"
#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "ISceneClass.h"
#include "SceneFactoryClass.h"
#include "SceneManagerClass.h"

bool Scene::SceneManagerClass::IsInitialize = false;

Scene::SceneManagerClass::SceneManagerClass()
{
	assert(!IsInitialize);

	m_ObjectManager = std::make_unique<Object::ObjectManagerClass>();
	assert(m_ObjectManager);

	m_TextManager = std::make_unique<Text::TextManagerClass>();
	assert(m_TextManager);

	m_UIManager = std::make_unique<UI::UIManagerClass>();
	assert(m_UIManager);

	m_SoundManager = std::make_unique<Sound::SoundManagerClass>();
	assert(m_SoundManager);

	m_Loader = std::make_unique<SceneFactoryClass>();
	assert(m_Loader);

	m_Scene = std::move(m_Loader->CreateScene(SceneState::START, m_ObjectManager.get(), m_TextManager.get(), m_UIManager.get(), m_SoundManager.get()));
	assert(m_Scene);

	IsInitialize = true;
}

Scene::SceneManagerClass::~SceneManagerClass()
{
	IsInitialize = false;
}

bool Scene::SceneManagerClass::Frame(const System::InputClass* input, float frame_time)
{
	if (m_Scene->IsSceneEnded())
	{
		ChangeScene();
		return true;
	}
	
	m_SoundManager->Frame();
	m_Scene->Frame(input, m_ObjectManager.get(), m_TextManager.get(), m_UIManager.get(), m_SoundManager.get(), frame_time);
	return false;
}

Object::IObjectClass* Scene::SceneManagerClass::GetCamera()
{
	return m_Scene->GetActiveCamera();
}

void Scene::SceneManagerClass::ChangeScene()
{
	SceneState next = m_Scene->GetNextSceneState();

	m_Scene.reset();

	m_Scene = std::move(m_Loader->CreateScene(next, m_ObjectManager.get(), m_TextManager.get(), m_UIManager.get(), m_SoundManager.get()));
}