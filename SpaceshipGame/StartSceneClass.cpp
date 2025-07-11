#include "pch.h"
#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"

#include "CameraClass.h"
#include "MoveClass.h"
#include "RotateClass.h"

#include "ActorClass.h"
#include "IUIClass.h"
#include "ITextClass.h"
#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(SceneState next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds) : SceneClass(Scene::SceneState::START, next)
{
	UINT objectIdx = 0;
	UINT UI_Idx = 0;
	UINT textIdx = 0;
	std::unique_ptr<Object::IMoveClass> move = std::make_unique<Object::MoveClass>();
	std::unique_ptr<Object::IRotateClass> rotate = std::make_unique<Object::RotateClass>();

	m_Camera = std::make_unique<Object::CameraClass>(std::move(move), std::move(rotate));
	assert(m_Camera);
	m_Camera->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	objectIdx = objects->Load(Object::ID::ACTOR, Graphic::Model::ID::DEFAULT_SPACESHIP);
	objects->SetPlayerIdx(objectIdx);
	objects->GetGameObject(objectIdx)->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	// UI_Idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	// UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	// UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(0.25f, 0.25f));

	// UI_Idx = UIs->LoadUI(UI::ID::DEFAULT, Graphic::Texture::UITextureID::START_BUTTON);
	// UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	// UIs->GetUI(UI_Idx)->SetPosition(DirectX::XMFLOAT2(0.f, 0.f));
	// UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(0.3f, 0.3f));
	
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
	Object::CameraClass* cam = nullptr;
	long x = 0, y = 0;

	if (input->IsSpacebarPressed())
	{
		SetSceneEnded();
		return;
	}

	cam = static_cast<Object::CameraClass*>(m_Camera.get());
	cam->Move(Object::MoveState::MOVE_FORWARD, frame_time, input->IsWBottunPressed());
	cam->Move(Object::MoveState::MOVE_BACKWARD, frame_time, input->IsSBottunPressed());
	cam->Move(Object::MoveState::MOVE_LEFT, frame_time, input->IsABottunPressed());
	cam->Move(Object::MoveState::MOVE_RIGHT, frame_time, input->IsDBottunPressed());

	input->GetMouseMoveDelta(x, y);
	cam->Rotate(x, y, frame_time, input->IsMouseCenterBottunPressed());
}
