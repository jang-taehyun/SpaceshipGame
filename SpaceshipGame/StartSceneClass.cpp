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
	UINT objectIdx = 0;
	UINT UI_Idx = 0;
	UINT textIdx = 0;
	UINT SoundMask = 0;
	std::unique_ptr<Object::IMoveClass> move = std::make_unique<Object::MoveClass>();
	std::unique_ptr<Object::IRotateClass> rotate = std::make_unique<Object::RotateClass>();

	UI_Idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(0.25f, 0.25f));

	UI_Idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(UI_Idx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	m_StartButtonIdx = UI_Idx;
	
	textIdx = texts->Load(Text::ID::DEFAULT, _T("시작하기"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(textIdx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	texts->GetTextObject(textIdx)->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 1.f, 1.f));
	texts->GetTextObject(textIdx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));

	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
	sounds->Play(Sound::ID::BACKGROUND);
}

Scene::StartSceneClass::StartSceneClass(const StartSceneClass& other) : SceneClass(other)
{}

Scene::StartSceneClass::StartSceneClass(StartSceneClass&& other) noexcept : SceneClass(std::move(other))
{}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(const StartSceneClass& other)
{
	if (this == &other)
		return *this;

	SceneClass::operator=(other);

	return *this;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(StartSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::StartSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	UI::ButtonClass* button = static_cast<UI::ButtonClass*>(UIs->GetUI(m_StartButtonIdx));

	if (UI::ButtonState::ONCLICKED == button->GetButtonState())
		SetSceneEnded();
}
