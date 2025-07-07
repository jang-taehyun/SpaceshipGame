#include "pch.h"
#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"
#include "CameraClass.h"
#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(SceneState next, Object::ObjectManagerClass* actors, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds) : SceneClass(Scene::SceneState::START, next)
{
	m_Camera = std::make_unique<Object::CameraClass>();
}

Scene::StartSceneClass::StartSceneClass(const StartSceneClass& other) : SceneClass(other), m_Camera(std::move(other.m_Camera->Clone())) {}
Scene::StartSceneClass::StartSceneClass(StartSceneClass&& other) noexcept : SceneClass(other), m_Camera(std::move(other.m_Camera)) {}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(const StartSceneClass& other)
{
	if (this == &other)
		return *this;

	m_Camera = other.m_Camera->Clone();
	SceneClass::operator=(other);

	return *this;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(StartSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_Camera = std::move(other.m_Camera);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::StartSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	if (input->IsSpacebarPressed())
		SetSceneEnded();
}
