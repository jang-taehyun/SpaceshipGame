#include "pch.h"

#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"

#include "ActorClass.h"

#include "IUIClass.h"
#include "ButtonClass.h"

#include "ITextClass.h"

#include "StartSceneClass.h"

Scene::StartSceneClass::StartSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::START, next)
{
	LoadSound(sounds);
	Load2DBackground(UIs);
	LoadLogo(UIs);
	LoadGameStartButton(texts, UIs);
	LoadESCPopupWindow(texts, UIs);
}

Scene::StartSceneClass::StartSceneClass(const StartSceneClass& other)
	: SceneClass(other),
	m_ESCPopupActive(other.m_ESCPopupActive)
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::StartSceneClass::StartSceneClass(StartSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_ObjectList(std::move(other.m_ObjectList)),
	m_ESCPopupActive(other.m_ESCPopupActive)
{}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(const StartSceneClass& other)
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	m_ESCPopupActive = other.m_ESCPopupActive;

	SceneClass::operator=(other);

	return *this;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(StartSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList = std::move(other.m_ObjectList);
	m_ESCPopupActive = other.m_ESCPopupActive;

	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::StartSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	if (ProcessStartButton(UIs->GetUI(m_ObjectList.find(ObjectID::SCENE_BUTTON_UI_START)->second)))
		return;

	if (ProcessESCPopUp(input, texts, UIs))
		return;
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

	// sound 로드
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
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
	// pos.y = (rc.bottom - rc.top) * 0.5f;

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

void Scene::StartSceneClass::LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT idx = 0;
	DirectX::XMFLOAT2 pos = { 0.f, 0.f };
	DirectX::XMFLOAT4 textColor = { 0.f, 0.f, 0.f, 1.f };
	float depth = 0.2f;

	// 화면의 정중앙 계산
	RECT rc = {};
	GetClientRect(System::hWnd, &rc);
	pos.x = (rc.right - rc.left) * 0.5f;
	pos.y = (rc.bottom - rc.top) * 0.5f;

	// 팝업창 background 로드
	idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::ECS_POPUP_BACKGROUND);
	UIs->GetUI(idx)->SetColor(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.5f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	UIs->GetUI(idx)->SetDepth(depth);
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BACKGROUND, idx));

	// 팝업창 UI 로드
	depth = 0.3f;
	idx = UIs->LoadUI(UI::ID::STATIC, Graphic::Texture::UITextureID::ECS_POPUP);
	UIs->GetUI(idx)->SetPosition(pos);
	UIs->GetUI(idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(300.f, 300.f));
	UIs->GetUI(idx)->SetDepth(depth);
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_UI, idx));

	// 팝업창 text 로드
	pos.y -= 60.f;
	idx = texts->Load(Text::ID::DEFAULT, _T("게임을 끝내시겠습니까?"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_UI_TEXT, idx));
	pos.y += 60.f;

	// 팝업창의 OK 버튼 UI 로드
	pos.y += 10.f;
	pos.x -= 60.f;
	idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(pos);
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(100.f, 60.f));
	UIs->GetUI(idx)->SetDepth(depth);
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_UI_OK, idx));

	// 팝업창의 OK 버튼의 text 로드
	idx = texts->Load(Text::ID::DEFAULT, _T("OK"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_TEXT_OK, idx));
	pos.y -= 10.f;
	pos.x += 60.f;

	// 팝업창의 CANCEL 버튼 UI 로드
	pos.y += 10.f;
	pos.x += 60.f;
	idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(pos);
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(100.f, 60.f));
	UIs->GetUI(idx)->SetDepth(depth);
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_UI_CANCEL, idx));

	// 팝업창의 CANCEL 버튼의 text 로드
	idx = texts->Load(Text::ID::DEFAULT, _T("CANCEL"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_TEXT_CANCEL, idx));
	pos.y += 10.f;
	pos.x -= 60.f;
}

void Scene::StartSceneClass::ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	if (m_ESCPopupActive)
		return;

	std::map<ObjectID, UINT>::iterator iter(m_ObjectList.begin());
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
		case ObjectID::POPUP_BACKGROUND:
		case ObjectID::POPUP_UI:
		case ObjectID::POPUP_BUTTON_UI_OK:
		case ObjectID::POPUP_BUTTON_UI_CANCEL:
		{
			UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, true);
			UIs->GetUI(iter->second)->SetUIState(UI::UIState::APPEAR, true);
			assert(UIs->GetUI(iter->second)->GetUIState() & (1 << static_cast<UINT>(UI::UIState::APPEAR)));
			break;
		}
		case ObjectID::POPUP_UI_TEXT:
		case ObjectID::POPUP_BUTTON_TEXT_OK:
		case ObjectID::POPUP_BUTTON_TEXT_CANCEL:
		{
			texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, true);
			texts->GetTextObject(iter->second)->SetTextState(UI::UIState::APPEAR, true);
			assert(texts->GetTextObject(iter->second)->GetTextState() & (1 << static_cast<UINT>(UI::UIState::APPEAR)));
			break;
		}
		case ObjectID::SCENE_BUTTON_TEXT_START:
		{
			texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, false);
			break;
		}
		default:
		{
			UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, false);
			break;
		}
		}
	}

	m_ESCPopupActive = true;
}

void Scene::StartSceneClass::DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	if (!m_ESCPopupActive)
		return;

	std::map<ObjectID, UINT>::iterator iter(m_ObjectList.begin());
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
		case ObjectID::POPUP_BACKGROUND:
		case ObjectID::POPUP_UI:
		case ObjectID::POPUP_BUTTON_UI_OK: 
		case ObjectID::POPUP_BUTTON_UI_CANCEL: 
		{
			UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, false);
			UIs->GetUI(iter->second)->SetUIState(UI::UIState::APPEAR, false);
			break;
		}
		case ObjectID::POPUP_UI_TEXT:
		case ObjectID::POPUP_BUTTON_TEXT_OK:
		case ObjectID::POPUP_BUTTON_TEXT_CANCEL:
		{
			texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, false);
			texts->GetTextObject(iter->second)->SetTextState(UI::UIState::APPEAR, false);
			break;
		}
		case ObjectID::SCENE_BUTTON_TEXT_START:
		{
			texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, true);
			break;
		}
		default:
		{
			UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, true);
			break;
		}
		}
	}

	m_ESCPopupActive = false;
}

bool Scene::StartSceneClass::ProcessStartButton(UI::IUIClass* button)
{
	UI::ButtonClass* b(static_cast<UI::ButtonClass*>(button));

	// button의 상태 가져오기
	UINT uiState(button->GetUIState());

	// button이 활성화 되었는지 검사
	bool IsActive(uiState & (1 << static_cast<UINT>(UI::UIState::ACTIVE)));

	// 버튼이 눌리면 다음 scene으로 전환
	if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
	{
		SetSceneEnded();
		return true;
	}
		
	return false;
}

bool Scene::StartSceneClass::ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UI::ButtonClass* b = nullptr;
	UINT uiState = 0;
	bool IsActive = false;

	// ESC 키가 눌리면 팝업창을 활성화하거나 비활성화
	if (input->IsEscapePressed())
	{
		if (!m_ESCPopupActive)
			ActiveESCPopup(texts, UIs);
		else
			DeactiveESCPopup(texts, UIs);

		return true;
	}

	// 팝업창이 활성화된 경우
	if (m_ESCPopupActive)
	{
		// OK 버튼이 눌렸는지 검사
		b = static_cast<UI::ButtonClass*>(
			UIs->GetUI(m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_OK)->second)
			);
		uiState = b->GetUIState();
		IsActive = uiState & (1 << static_cast<UINT>(UI::UIState::ACTIVE));

		// 버튼이 눌리면 프로그램 종료
		if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
		{
			System::RUNNING = false;
			return true;
		}

		// CANCEL 버튼이 눌렸는지 검사
		b = static_cast<UI::ButtonClass*>(
			UIs->GetUI(m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_CANCEL)->second)
			);
		uiState = b->GetUIState();
		IsActive = uiState & (1 << static_cast<UINT>(UI::UIState::ACTIVE));

		// 버튼이 눌리면 팝업창 비활성화
		if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
		{
			DeactiveESCPopup(texts, UIs);
			return true;
		}
	}

	return false;
}
