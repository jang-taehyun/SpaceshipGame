#include "pch.h"
#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"
#include "CameraClass.h"

#include "ActorClass.h"
#include "IUIClass.h"
#include "ITextClass.h"
#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(SceneState next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds) : SceneClass(Scene::SceneState::START, next)
{
	UINT objectIdx = 0;
	UINT UI_Idx = 0;
	UINT textIdx = 0;

	m_Camera = std::make_unique<Object::CameraClass>();
	assert(m_Camera);
	m_Camera->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, -15.f, 1.f));

	objectIdx = objects->Load(Object::ID::ACTOR, Graphic::Model::ID::DEFAULT_SPACESHIP);
	objects->SetPlayerIdx(objectIdx);
	objects->GetGameObject(objectIdx)->SetPosition(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));

	UI_Idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));

	UI_Idx = UIs->LoadUI(UI::ID::DEFAULT, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(UI_Idx)->SetPosition(DirectX::XMFLOAT2(50.f, 50.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(0.015f, 0.015f));
	
	textIdx = texts->Load(Text::ID::DEFAULT, _T("~~우주선 게임~~"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(textIdx)->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	texts->GetTextObject(textIdx)->SetScale(DirectX::XMFLOAT2(1.f, 2.f));
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
	{
		SetSceneEnded();
		return;
	}

	static_cast<Object::ActorClass*>(
		objects->GetGameObject(objects->GetPlayerIdx())
		)->Move(Object::MoveState::MOVE_FORWARD, frame_time, input->IsWBottunPressed());

	static_cast<Object::ActorClass*>(
		objects->GetGameObject(objects->GetPlayerIdx())
		)->Move(Object::MoveState::MOVE_BACKWARD, frame_time, input->IsSBottunPressed());

	static_cast<Object::ActorClass*>(
		objects->GetGameObject(objects->GetPlayerIdx())
		)->Move(Object::MoveState::MOVE_LEFT, frame_time, input->IsABottunPressed());

	static_cast<Object::ActorClass*>(
		objects->GetGameObject(objects->GetPlayerIdx())
		)->Move(Object::MoveState::MOVE_RIGHT, frame_time, input->IsDBottunPressed());
}
