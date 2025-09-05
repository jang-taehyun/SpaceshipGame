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
#include "SingleModeSceneClass.h"

Scene::SingleModeSceneClass::SingleModeSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::SINGLE_MODE, next, texts, UIs)
{
	LoadESCPopupWindow(texts, UIs);
	LoadSound(sounds);
	LoadTerrain();
	LoadObject(objects);
}

Scene::SingleModeSceneClass::SingleModeSceneClass(const SingleModeSceneClass& other)
	: SceneClass(other)
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::SingleModeSceneClass::SingleModeSceneClass(SingleModeSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_ObjectList(std::move(other.m_ObjectList))
{}

Scene::SingleModeSceneClass& Scene::SingleModeSceneClass::operator=(const SingleModeSceneClass& other)
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	SceneClass::operator=(other);

	return *this;
}

Scene::SingleModeSceneClass& Scene::SingleModeSceneClass::operator=(SingleModeSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList = std::move(other.m_ObjectList);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::SingleModeSceneClass::LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT idx = 0;
	DirectX::XMFLOAT2 pos = { 0.f, 0.f };
	DirectX::XMFLOAT4 textColor = { 0.f, 0.f, 0.f, 1.f };
	float depth = 0.2f;
	RECT rc = {};

	// 부모 클래스의 LoadESCPopupWindow() 함수 호출
	SceneClass::LoadESCPopupWindow(texts, UIs);

	// 화면의 정중앙 계산
	GetClientRect(System::hWnd, &rc);
	pos.x = (rc.right - rc.left) * 0.5f;
	pos.y = (rc.bottom - rc.top) * 0.5f;

	// 팝업창의 PREVIOUS 버튼 UI 로드
	pos.y += 80.f;
	idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(pos);
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(170.f, 60.f));
	UIs->GetUI(idx)->SetDepth(depth);
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_UI_PREVIOUS, idx));

	// 팝업창의 PREVIOUS 버튼의 text 로드
	idx = texts->Load(Text::ID::DEFAULT, _T("이전 화면으로"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
	m_ObjectList.insert(std::make_pair(ObjectID::POPUP_BUTTON_TEXT_PREVIOUS, idx));
	pos.y -= 80.f;
}

bool Scene::SingleModeSceneClass::ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UI::ButtonClass* b = nullptr;
	UINT uiState = 0;
	bool IsActive = false;

	// 부모 클래스의 ProcessESCPopUp() 함수 호출
	if (SceneClass::ProcessESCPopUp(input, texts, UIs))
		return true;

	// 팝업창이 활성화된 경우
	if (GetIsESCPopupActive())
	{
		// PREVIOUS 버튼이 눌렸는지 검사
		b = static_cast<UI::ButtonClass*>(
			UIs->GetUI(m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_PREVIOUS)->second)
			);
		uiState = b->GetUIState();
		IsActive = uiState & (1 << static_cast<UINT>(UI::UIState::ACTIVE));

		// 버튼이 눌리면 팝업창을 닫고, 다음 scene으로 전환
		if (IsActive && UI::ButtonState::ONCLICKED == b->GetButtonState())
		{
			// 팝업창 닫기
			DeactiveESCPopup(texts, UIs);
			ActiveChildSceneUI(texts, UIs);

			// 다음 scene으로 전환
			if (ID::MODE_CHOOSE != GetNextSceneID())
				SetNextScene(ID::MODE_CHOOSE);
			SetSceneEnded();

			return true;
		}
	}

	return false;
}

void Scene::SingleModeSceneClass::ActiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT idx = 0;

	// 부모 클래스의 ActiveESCPopup() 함수 호출
	SceneClass::ActiveESCPopup(texts, UIs);

	// PRVIOUS 버튼 활성화
	idx = m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_PREVIOUS)->second;
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, true);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, true);

	idx = m_ObjectList.find(ObjectID::POPUP_BUTTON_TEXT_PREVIOUS)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, true);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, true);
}

void Scene::SingleModeSceneClass::DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT idx = 0;

	// 부모 클래스의 DeactiveESCPopup() 함수 호출
	SceneClass::DeactiveESCPopup(texts, UIs);

	// PRVIOUS 버튼 비활성화
	idx = m_ObjectList.find(ObjectID::POPUP_BUTTON_UI_PREVIOUS)->second;
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);

	idx = m_ObjectList.find(ObjectID::POPUP_BUTTON_TEXT_PREVIOUS)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
}

void Scene::SingleModeSceneClass::ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
}

void Scene::SingleModeSceneClass::DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
}

void Scene::SingleModeSceneClass::ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	objects->Frame(input, texts, sounds, frame_time, GetIsESCPopupActive());
}

void Scene::SingleModeSceneClass::LoadSound(Sound::SoundManagerClass* sounds)
{
	UINT SoundMask = 0;

	// sound 로드 및 재생
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
	sounds->SetLoop(Sound::ID::BACKGROUND, true);
	sounds->Play(Sound::ID::BACKGROUND);
}

void Scene::SingleModeSceneClass::LoadTerrain()
{
	// 지형 로드
	SetTerrainID(Graphic::Terrain::TerrainID::DEFAULT);
	SetSkyDomeID(Graphic::Terrain::SkyDomeID::DEFAULT);
}

void Scene::SingleModeSceneClass::LoadObject(Object::ObjectManagerClass* objects)
{
	// 카메라 로드
	UINT idx = objects->Load(Object::ID::CAMERA, Graphic::Model::ID::NONE);
	objects->SetCameraIdx(idx);
	objects->GetGameObject(idx)->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	// object 로드
	idx = objects->Load(Object::ID::ACTOR, Graphic::Model::ID::DEFAULT_SPACESHIP);
	objects->GetGameObject(idx)->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));
}