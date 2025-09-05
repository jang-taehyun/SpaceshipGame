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

	IsInitialize = true;
}

Scene::SceneManagerClass::~SceneManagerClass()
{
	IsInitialize = false;
}

bool Scene::SceneManagerClass::Frame(const System::InputClass* input, float frame_time)
{
	if (!m_Scene)
	{
		m_Scene = std::move(m_Loader->CreateScene(ID::START, m_ObjectManager.get(), m_TextManager.get(), m_UIManager.get(), m_SoundManager.get()));
		assert(m_Scene);
		return true;
	}

	if (m_Scene->IsSceneEnded())
	{
		ChangeScene(m_Scene->GetNextSceneID());
		return true;
	}
	
	m_SoundManager->Frame();
	m_UIManager->Frame(input);
	m_Scene->Frame(input, m_ObjectManager.get(), m_TextManager.get(), m_UIManager.get(), m_SoundManager.get(), frame_time);
	return false;
}

void Scene::SceneManagerClass::Release()
{
	m_ObjectManager->Release();
	m_TextManager->Release();
	m_UIManager->Release();
}

void Scene::SceneManagerClass::ChangeScene(ID next)
{
	// 이전 scene에서 사용한 객체 소멸
	m_SoundManager->AllStop();
	Release();
	m_Scene.reset();

	// 다음 scene의 ID를 통해 scene 생성
	m_Scene = std::move(m_Loader->CreateScene(next, m_ObjectManager.get(), m_TextManager.get(), m_UIManager.get(), m_SoundManager.get()));
}