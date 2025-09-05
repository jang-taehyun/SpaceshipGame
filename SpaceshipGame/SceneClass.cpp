#include "pch.h"
#include "InputClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "IUIClass.h"
#include "ButtonClass.h"
#include "ITextClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(ID current, ID next, Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
	: m_CurrentSceneID(current),
	m_NextSceneID(next)
{}

Scene::SceneClass::SceneClass(const SceneClass & other)
	: m_CurrentSceneID(other.m_CurrentSceneID),
	m_NextSceneID(other.m_NextSceneID),
	m_TerrainID(other.m_TerrainID),
	m_SkyDomeID(other.m_SkyDomeID),
	m_IsSceneEnded(other.m_IsSceneEnded),
	m_ESCPopupActive(other.m_ESCPopupActive)
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::SceneClass::SceneClass(SceneClass&& other) noexcept
	: m_CurrentSceneID(other.m_CurrentSceneID),
	m_NextSceneID(other.m_NextSceneID),
	m_TerrainID(other.m_TerrainID),
	m_SkyDomeID(other.m_SkyDomeID),
	m_IsSceneEnded(other.m_IsSceneEnded),
	m_ESCPopupActive(other.m_ESCPopupActive),
	m_ObjectList(std::move(other.m_ObjectList))
{}

Scene::SceneClass::~SceneClass()
{
	SetShowMouseCursor(true);
}

Scene::SceneClass& Scene::SceneClass::operator=(const SceneClass& other)
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_CurrentSceneID = other.m_CurrentSceneID;
	m_NextSceneID = other.m_NextSceneID;
	m_TerrainID = other.m_TerrainID;
	m_SkyDomeID = other.m_SkyDomeID;
	m_IsSceneEnded = other.m_IsSceneEnded;
	m_ESCPopupActive = other.m_ESCPopupActive;
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());

	return *this;
}

Scene::SceneClass& Scene::SceneClass::operator=(SceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_CurrentSceneID = other.m_CurrentSceneID;
	m_NextSceneID = other.m_NextSceneID;
	m_TerrainID = other.m_TerrainID;
	m_SkyDomeID = other.m_SkyDomeID;
	m_IsSceneEnded = other.m_IsSceneEnded;
	m_ESCPopupActive = other.m_ESCPopupActive;
	m_ObjectList = std::move(m_ObjectList);

	return *this;
}

void Scene::SceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	if (ProcessESCPopUp(input, texts, UIs))
		return;

	ProcessChildScene(input, objects, texts, UIs, sounds, frame_time);
}

bool Scene::SceneClass::GetIsShowMouseCursor() const
{
	CURSORINFO cursorInfo = {};
	cursorInfo.cbSize = sizeof(cursorInfo);

	if (GetCursorInfo(&cursorInfo))
	{
		// 현재 마우스 커서가 보이지 않음
		if (!cursorInfo.flags)
			return false;

		// 현재 마우스 커서가 보임
		else
			return true;
	}

	assert(false);
	return false;
}

void Scene::SceneClass::SetShowMouseCursor(bool IsShow)
{
	// 현재 마우스 커서가 보이지 않는다면 보이게 하기 //
	if (!GetIsShowMouseCursor() && IsShow)
		ShowCursor(true);

	// 현재 마우스 커서가 보인다면 안 보이게 하기 //
	else if (GetIsShowMouseCursor() && !IsShow)
	{
		while (GetIsShowMouseCursor())
			ShowCursor(false);
	}
}

void Scene::SceneClass::LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
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
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
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
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
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
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_TEXT_CANCEL, idx));
	pos.y += 10.f;
	pos.x -= 60.f;
}


void Scene::SceneClass::ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	std::map<ObjectID, UINT>::iterator iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
			// 팝업창 UI 활성화
			case ObjectID::POPUP_BACKGROUND:
			case ObjectID::POPUP_UI:
			case ObjectID::POPUP_BUTTON_UI_OK:
			case ObjectID::POPUP_BUTTON_UI_CANCEL:
			{
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, true);
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::APPEAR, true);
				break;
			}

			// 팝업창 text 활성화
			case ObjectID::POPUP_UI_TEXT:
			case ObjectID::POPUP_BUTTON_TEXT_OK:
			case ObjectID::POPUP_BUTTON_TEXT_CANCEL:
			{
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, true);
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::APPEAR, true);
				break;
			}
		}
	}

	m_ESCPopupActive = true;
}

void Scene::SceneClass::DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	std::map<ObjectID, UINT>::iterator iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
			// 팝업창 UI 비활성화
			case ObjectID::POPUP_BACKGROUND:
			case ObjectID::POPUP_UI:
			case ObjectID::POPUP_BUTTON_UI_OK:
			case ObjectID::POPUP_BUTTON_UI_CANCEL:
			{
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, false);
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::APPEAR, false);
				break;
			}

			// 팝업창 text 활성화
			case ObjectID::POPUP_UI_TEXT:
			case ObjectID::POPUP_BUTTON_TEXT_OK:
			case ObjectID::POPUP_BUTTON_TEXT_CANCEL:
			{
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, false);
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::APPEAR, false);
				break;
			}
		}
	}

	m_ESCPopupActive = false;
}


bool Scene::SceneClass::ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UI::ButtonClass* b = nullptr;
	UINT idx = 0;
	UINT ActiveFlag = (1 << static_cast<UINT>(UI::UIState::ACTIVE));
	UINT uiState = 0;
	bool IsActive = false;

	// ESC 키가 눌리면 팝업창을 활성화하거나 비활성화
	if (System::KEYSTATE::AWAY == input->GetKeyState(System::KEY::ESC))
	{
		if (!m_ESCPopupActive)
		{
			DeactiveChildSceneUI(texts, UIs);
			ActiveESCPopup(texts, UIs);
		}
		else
		{
			DeactiveESCPopup(texts, UIs);
			ActiveChildSceneUI(texts, UIs);
		}

		return true;
	}

	// 팝업창이 활성화된 경우
	if (m_ESCPopupActive)
	{
		// OK 버튼이 눌렸는지 검사
		idx = m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_OK)->second;
		b = static_cast<UI::ButtonClass*>(UIs->GetUI(idx));
		uiState = b->GetUIState();
		IsActive = uiState & ActiveFlag;

		// 버튼이 눌리면 프로그램 종료
		if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
		{
			System::RUNNING = false;
			return true;
		}

		// CANCEL 버튼이 눌렸는지 검사
		idx = m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_CANCEL)->second;
		b = static_cast<UI::ButtonClass*>(UIs->GetUI(idx));
		uiState = b->GetUIState();
		IsActive = uiState & ActiveFlag;

		// 버튼이 눌리면 팝업창 비활성화
		if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
		{
			DeactiveESCPopup(texts, UIs);
			ActiveChildSceneUI(texts, UIs);
			return true;
		}
	}

	return false;
}