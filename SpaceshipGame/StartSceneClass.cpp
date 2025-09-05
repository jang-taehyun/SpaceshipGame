#include "pch.h"

#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"

#include "IUIClass.h"
#include "ButtonClass.h"

#include "ITextClass.h"

#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::START, next, texts, UIs)
{
	LoadESCPopupWindow(texts, UIs);
	LoadSound(sounds);
	Load2DBackground(UIs);
	LoadLogo(UIs);
	LoadGameStartButton(texts, UIs);
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

	m_ObjectList.clear();

	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	SceneClass::operator=(other);

	return *this;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(StartSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList = std::move(other.m_ObjectList);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::StartSceneClass::ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	ProcessStartButton(UIs->GetUI(m_ObjectList.find(ObjectID::SCENE_BUTTON_UI_START)->second));
}

void Scene::StartSceneClass::ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	std::map<ObjectID, UINT>::iterator iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
			// scene의 UI 활성화
			case ObjectID::SCENE_BUTTON_UI_START:
			case ObjectID::SCENE_BACKGROUND:
			case ObjectID::SCENE_LOGO:
			{
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, true);
				break;
			}

			// scene의 text 활성화
			case ObjectID::SCENE_BUTTON_TEXT_START:
			{
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, true);
				break;
			}
		}
	}
}

void Scene::StartSceneClass::DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	std::map<ObjectID, UINT>::iterator iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
			// scene의 UI 비활성화
			case ObjectID::SCENE_BUTTON_UI_START:
			case ObjectID::SCENE_BACKGROUND:
			case ObjectID::SCENE_LOGO:
			{
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, false);
				break;
			}

			// scene의 text 비활성화
			case ObjectID::SCENE_BUTTON_TEXT_START:
			{
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, false);
				break;
			}
		}
	}
}

void Scene::StartSceneClass::LoadLogo(UI::UIManagerClass* UIs)
{
	float depth = 0.1f;
	DirectX::XMFLOAT2 pos(600.f, 250.f);

	// 게임 로고 로드
	UINT idx = UIs->LoadUI(UI::ID::STATIC, Graphic::Texture::UITextureID::LOGO);
	UIs->GetUI(idx)->SetPosition(pos);
	UIs->GetUI(idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(300.f, 300.f));
	UIs->GetUI(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_LOGO, idx));
}

void Scene::StartSceneClass::LoadSound(Sound::SoundManagerClass* sounds)
{
	UINT SoundMask = 0;

	// sound 로드 및 재생
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
	sounds->SetLoop(Sound::ID::BACKGROUND, true);
	sounds->Play(Sound::ID::BACKGROUND);
}

void Scene::StartSceneClass::Load2DBackground(UI::UIManagerClass* UIs)
{
	float depth = 0.f;

	// 백그라운드 로드
	UINT idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	UIs->GetUI(idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(0.25f, 0.25f));
	UIs->GetUI(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BACKGROUND, idx));
}

void Scene::StartSceneClass::LoadGameStartButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	DirectX::XMFLOAT2 pos = { 600.f, 550.f };
	DirectX::XMFLOAT4 textColor = { 0.f, 0.f, 0.f, 1.f };
	float depth = 0.1f;

	// 화면의 정중앙 계산
	RECT rc = {};
	GetClientRect(System::hWnd, &rc);
	pos.x = (rc.right - rc.left) * 0.5f;

	// 시작 버튼 UI 로드
	UINT idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(pos);
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	UIs->GetUI(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BUTTON_UI_START, idx));

	// 시작 버튼의 text 로드
	idx = texts->Load(Text::ID::DEFAULT, _T("시작하기"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BUTTON_TEXT_START, idx));
}

void Scene::StartSceneClass::ProcessStartButton(UI::IUIClass* button)
{
	// button UI 가져오기
	UI::ButtonClass* b = static_cast<UI::ButtonClass*>(button);
	assert(b);

	// button의 상태 가져오기
	UINT uiState = button->GetUIState();

	// button이 활성화 되었는지 검사
	bool IsActive = uiState & (1 << static_cast<UINT>(UI::UIState::ACTIVE));

	// 버튼이 눌리면 다음 scene으로 전환
	if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
		SetSceneEnded();
}

