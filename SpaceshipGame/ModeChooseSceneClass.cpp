#include "pch.h"
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
#include "ModeChooseSceneClass.h"

Scene::ModeChooseSceneClass::ModeChooseSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::MODE_CHOOSE, next)
{
	UINT objectIdx = 0;
	UINT UI_Idx = 0;
	UINT textIdx = 0;
	UINT SoundMask = 0;

	// 백그라운드 생성 //
	UI_Idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	UIs->GetUI(UI_Idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(0.25f, 0.25f));

	// 싱글 플레이 모드 버튼 생성 //
	UI_Idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(UI_Idx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	m_SingleModeButtonIdx = UI_Idx;

	textIdx = texts->Load(Text::ID::DEFAULT, _T("싱글 플레이"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(textIdx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	texts->GetTextObject(textIdx)->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 1.f, 1.f));
	texts->GetTextObject(textIdx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));

	// 멀티 플레이 모드 버튼 생성 //
	UI_Idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(UI_Idx)->SetPosition(DirectX::XMFLOAT2(600.f, 350.f));
	UIs->GetUI(UI_Idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	m_MultiModeButtonIdx = UI_Idx;

	textIdx = texts->Load(Text::ID::DEFAULT, _T("멀티 플레이(활성화 되지 않음)"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(textIdx)->SetPosition(DirectX::XMFLOAT2(600.f, 350.f));
	texts->GetTextObject(textIdx)->SetColor(DirectX::XMFLOAT4(0.f, 1.f, 1.f, 1.f));
	texts->GetTextObject(textIdx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));

	// 사운드 생성 //
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
	sounds->Play(Sound::ID::BACKGROUND);
}

Scene::ModeChooseSceneClass::ModeChooseSceneClass(const ModeChooseSceneClass& other)
	: SceneClass(other)
{}

Scene::ModeChooseSceneClass::ModeChooseSceneClass(ModeChooseSceneClass&& other) noexcept
	: SceneClass(std::move(other))
{}

Scene::ModeChooseSceneClass& Scene::ModeChooseSceneClass::operator=(const ModeChooseSceneClass& other)
{
	if (this == &other)
		return *this;

	SceneClass::operator=(other);

	return *this;
}

Scene::ModeChooseSceneClass& Scene::ModeChooseSceneClass::operator=(ModeChooseSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::ModeChooseSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	UI::ButtonClass* singlebutton = static_cast<UI::ButtonClass*>(UIs->GetUI(m_SingleModeButtonIdx));
	UI::ButtonClass* multibutton = static_cast<UI::ButtonClass*>(UIs->GetUI(m_MultiModeButtonIdx));

	if (UI::ButtonState::ONCLICKED == singlebutton->GetButtonState())
	{
		SetNextScene(ID::SINGLE_MODE);
		SetSceneEnded();
		return;
	}

	if (UI::ButtonState::ONCLICKED == multibutton->GetButtonState())
	{
		// SetNextScene(ID::SINGLE_MODE);
		// SetSceneEnded();

		MessageBox(GetActiveWindow(), _T("멀티플레이 모드가 활성화되지 않았습니다."), _T("경고"), MB_OK);

		return;
	}
}