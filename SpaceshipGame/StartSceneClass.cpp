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
#include "ButtonClass.h"

#include "ITextClass.h"

#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::START, next)
{
	UINT idx = 0;
	UINT SoundMask = 0;

	// sound 로드
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
	sounds->Play(Sound::ID::BACKGROUND);

	// 백그라운드 로드
	idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	UIs->GetUI(idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(0.25f, 0.25f));
	m_ObjectList.insert(std::make_pair(ObjectID::UI_SCENE_BACKGROUND, idx));

	// 시작 버튼 로드
	idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	UIs->GetUI(idx)->SetDepth(1.f);
	m_ObjectList.insert(std::make_pair(ObjectID::UI_START_BUTTON, idx));
	
	// 시작 버튼의 text 로드
	idx = texts->Load(Text::ID::DEFAULT, _T("시작하기"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	texts->GetTextObject(idx)->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 1.f, 1.f));
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(1.f);
	m_ObjectList.insert(std::make_pair(ObjectID::TEXT_START_STRING, idx));

	
}

Scene::StartSceneClass::StartSceneClass(const StartSceneClass& other)
	: SceneClass(other)
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::StartSceneClass::StartSceneClass(StartSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_ObjectList(std::move(other.m_ObjectList))
{}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(const StartSceneClass& other)
{
	if (this == &other)
		return *this;

	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	SceneClass::operator=(other);

	return *this;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(StartSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ObjectList = std::move(other.m_ObjectList);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::StartSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	UI::ButtonClass* button(
		static_cast<UI::ButtonClass*>(
			UIs->GetUI(m_ObjectList.find(ObjectID::UI_START_BUTTON)->second)
			)
	);

	if (UI::ButtonState::ONCLICKED == button->GetButtonState())
		SetSceneEnded();
}