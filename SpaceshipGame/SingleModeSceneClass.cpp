#include "pch.h"
#include <algorithm>
#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"

#include "CameraClass.h"
#include "ActorClass.h"
#include "MoveClass.h"
#include "RotateClass.h"

#include "IUIClass.h"
#include "ButtonClass.h"

#include "ITextClass.h"
#include "SingleModeSceneClass.h"

Scene::SingleModeSceneClass::SingleModeSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::SINGLE_MODE, next)
{
	UINT objectIdx = 0;
	UINT UI_Idx = 0;
	UINT textIdx = 0;
	UINT SoundMask = 0;
	std::unique_ptr<Object::IMoveClass> move = std::make_unique<Object::MoveClass>();
	std::unique_ptr<Object::IRotateClass> rotate = std::make_unique<Object::RotateClass>();

	// 카메라 로드
	m_Camera = std::make_unique<Object::CameraClass>(std::move(move), std::move(rotate));
	assert(m_Camera);
	m_Camera->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	// 지형 로드
	SetTerrainID(Graphic::Terrain::TerrainID::DEFAULT);
	SetSkyDomeID(Graphic::Terrain::SkyDomeID::DEFAULT);

	// 사운드 로드
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::EFFECT));
	sounds->Load(SoundMask);
	sounds->SetLoop(Sound::ID::BACKGROUND, true);
	sounds->Play(Sound::ID::BACKGROUND);

	// object 로드
	objectIdx = objects->Load(Object::ID::ACTOR, Graphic::Model::ID::DEFAULT_SPACESHIP);
	objects->SetPlayerIdx(objectIdx);
	objects->GetGameObject(objectIdx)->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));
}


Scene::SingleModeSceneClass::SingleModeSceneClass(const SingleModeSceneClass& other)
	: SceneClass(other),
	m_Camera(std::move(other.m_Camera->Clone()))
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::SingleModeSceneClass::SingleModeSceneClass(SingleModeSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_Camera(std::move(other.m_Camera)),
	m_ObjectList(std::move(other.m_ObjectList))
{}

Scene::SingleModeSceneClass& Scene::SingleModeSceneClass::operator=(const SingleModeSceneClass& other)
{
	if (this == &other)
		return *this;

	m_Camera = other.m_Camera->Clone();
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	SceneClass::operator=(other);

	return *this;
}

Scene::SingleModeSceneClass& Scene::SingleModeSceneClass::operator=(SingleModeSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_Camera = std::move(other.m_Camera);
	m_ObjectList = std::move(other.m_ObjectList);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::SingleModeSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	Object::CameraClass* cam = nullptr;
	UI::ButtonClass* button = nullptr;
	long x = 0, y = 0;

	// button = static_cast<UI::ButtonClass*>(UIs->GetUI(0));
	// if (UI::ButtonState::ONCLICKED == button->GetButtonState())
	// {
	// 	SetSceneEnded();
	// 	return;
	// }

	cam = static_cast<Object::CameraClass*>(m_Camera.get());
	cam->Move(Object::MoveState::MOVE_FORWARD, frame_time, input->IsWBottunPressed());
	cam->Move(Object::MoveState::MOVE_BACKWARD, frame_time, input->IsSBottunPressed());
	cam->Move(Object::MoveState::MOVE_LEFT, frame_time, input->IsABottunPressed());
	cam->Move(Object::MoveState::MOVE_RIGHT, frame_time, input->IsDBottunPressed());

	input->GetMouseMoveDelta(x, y);
	cam->Rotate(x, y, frame_time, input->IsMouseCenterBottunPressed());
}

void Scene::SingleModeSceneClass::CreateECSPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT UI_Idx, textIdx;

	UI_Idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(UI_Idx)->SetPosition(DirectX::XMFLOAT2(50.f, 30.f));
	UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(50.f, 40.f));

	UIs->GetUI(UI_Idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(UI_Idx)->SetUIState(UI::UIState::APPEAR, false);

	textIdx = texts->Load(Text::ID::DEFAULT, _T("이전 화면으로"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(textIdx)->SetPosition(DirectX::XMFLOAT2(50.f, 30.f));
	texts->GetTextObject(textIdx)->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	texts->GetTextObject(textIdx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));

	texts->GetTextObject(textIdx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(textIdx)->SetTextState(UI::UIState::APPEAR, false);
}
