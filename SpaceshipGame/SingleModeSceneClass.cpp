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
	RECT rt = {};
	POINT p = {};

	// scene 내에 필요한 object들을 모두 로드
	LoadESCPopupWindow(texts, UIs);
	LoadSound(sounds);
	LoadTerrain();
	LoadScoreText(texts);
	LoadObject(objects);

	// 마우스 커서 위치를 정가운데로 초기화하고 안보이게 하기 
	GetClientRect(System::hWnd, &rt);
	p.x = (rt.right - rt.left) * 0.5f;
	p.y = (rt.bottom - rt.top) * 0.5f;
	SetCursorPos(p.x, p.y);
	SetShowMouseCursor(false);

	// 마우스 이동 범위 백업
	GetClipCursor(&m_OriginMouseCursorMoveRange);
}

Scene::SingleModeSceneClass::SingleModeSceneClass(const SingleModeSceneClass& other)
	: SceneClass(other),
	m_OriginMouseCursorMoveRange(other.m_OriginMouseCursorMoveRange),
	m_CurrentMouseCursorMoveRange(other.m_CurrentMouseCursorMoveRange)
{
	m_UIIdxList.insert(other.m_UIIdxList.begin(), other.m_UIIdxList.end());
	m_ObjectIdxList.insert(other.m_ObjectIdxList.begin(), other.m_ObjectIdxList.end());
}

Scene::SingleModeSceneClass::SingleModeSceneClass(SingleModeSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_UIIdxList(std::move(other.m_UIIdxList)),
	m_ObjectIdxList(std::move(other.m_ObjectIdxList)),
	m_OriginMouseCursorMoveRange(other.m_OriginMouseCursorMoveRange),
	m_CurrentMouseCursorMoveRange(other.m_CurrentMouseCursorMoveRange)
{}

Scene::SingleModeSceneClass::~SingleModeSceneClass()
{
	// 마우스 이동 범위 원상복구
	ClipCursor(&m_OriginMouseCursorMoveRange);
}

Scene::SingleModeSceneClass& Scene::SingleModeSceneClass::operator=(const SingleModeSceneClass& other)
{
	if (this == &other)
		return *this;

	m_UIIdxList.clear();
	m_ObjectIdxList.clear();

	m_UIIdxList.insert(other.m_UIIdxList.begin(), other.m_UIIdxList.end());
	m_ObjectIdxList.insert(other.m_ObjectIdxList.begin(), other.m_ObjectIdxList.end());
	m_OriginMouseCursorMoveRange = other.m_OriginMouseCursorMoveRange;
	m_CurrentMouseCursorMoveRange = other.m_CurrentMouseCursorMoveRange;

	SceneClass::operator=(other);

	return *this;
}

Scene::SingleModeSceneClass& Scene::SingleModeSceneClass::operator=(SingleModeSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_UIIdxList.clear();
	m_ObjectIdxList.clear();

	m_UIIdxList = std::move(other.m_UIIdxList);
	m_ObjectIdxList = std::move(other.m_ObjectIdxList);
	m_OriginMouseCursorMoveRange = other.m_OriginMouseCursorMoveRange;
	m_CurrentMouseCursorMoveRange = other.m_CurrentMouseCursorMoveRange;

	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::SingleModeSceneClass::LoadESCPopupWindow(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT idx = 0;
	DirectX::XMFLOAT2 pos = { 0.f, 0.f };
	DirectX::XMFLOAT4 textColor = { 0.f, 0.f, 0.f, 1.f };
	float depth = 0.22f;
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
	m_UIIdxList.insert(std::make_pair(ObjectID::POPUP_BUTTON_UI_PREVIOUS, idx));

	// 팝업창의 PREVIOUS 버튼의 text 로드
	idx = texts->Load(Text::ID::DEFAULT, _T("이전 화면으로"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);
	m_UIIdxList.insert(std::make_pair(ObjectID::POPUP_BUTTON_TEXT_PREVIOUS, idx));
	pos.y -= 80.f;
}

bool Scene::SingleModeSceneClass::ProcessESCPopUp(const System::InputClass* input, Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UI::ButtonClass* b = nullptr;
	UINT uiState = 0;
	bool IsActive = false;
	UINT idx = 0;

	// 설정한 마우스 이동 범위 유지
	LimitMouseCursorMoveRange();

	// 부모 클래스의 ProcessESCPopUp() 함수 호출
	if (SceneClass::ProcessESCPopUp(input, texts, UIs))
		return true;

	// 팝업창이 활성화된 경우
	if (GetIsESCPopupActive())
	{
		// PREVIOUS 버튼이 눌렸는지 검사
		idx = m_UIIdxList.find(ObjectID::POPUP_BUTTON_UI_PREVIOUS)->second;
		b = static_cast<UI::ButtonClass*>(UIs->GetUI(idx));
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
	idx = m_UIIdxList.find(ObjectID::POPUP_BUTTON_UI_PREVIOUS)->second;
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, true);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, true);

	idx = m_UIIdxList.find(ObjectID::POPUP_BUTTON_TEXT_PREVIOUS)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, true);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, true);

	SetShowMouseCursor(true);
}

void Scene::SingleModeSceneClass::DeactiveESCPopup(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	UINT idx = 0;

	// 부모 클래스의 DeactiveESCPopup() 함수 호출
	SceneClass::DeactiveESCPopup(texts, UIs);

	// PRVIOUS 버튼 비활성화
	idx = m_UIIdxList.find(ObjectID::POPUP_BUTTON_UI_PREVIOUS)->second;
	UIs->GetUI(idx)->SetUIState(UI::UIState::ACTIVE, false);
	UIs->GetUI(idx)->SetUIState(UI::UIState::APPEAR, false);

	idx = m_UIIdxList.find(ObjectID::POPUP_BUTTON_TEXT_PREVIOUS)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
	texts->GetTextObject(idx)->SetTextState(UI::UIState::APPEAR, false);

	SetShowMouseCursor(false);
}

void Scene::SingleModeSceneClass::ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	// 스코어 텍스트 활성화
	UINT idx = m_UIIdxList.find(ObjectID::SCENE_TEXT_SCORE_TITLE)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, true);

	idx = m_UIIdxList.find(ObjectID::SCENE_TEXT_SCORE)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, true);
}

void Scene::SingleModeSceneClass::DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	// 스코어 텍스트 비활성화
	UINT idx = m_UIIdxList.find(ObjectID::SCENE_TEXT_SCORE_TITLE)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);

	idx = m_UIIdxList.find(ObjectID::SCENE_TEXT_SCORE)->second;
	texts->GetTextObject(idx)->SetTextState(UI::UIState::ACTIVE, false);
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
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::EFFECT));
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

void Scene::SingleModeSceneClass::LoadScoreText(Text::TextManagerClass* texts)
{
	DirectX::XMFLOAT2 pos = { 45.f, 25.f };
	DirectX::XMFLOAT4 color = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT2 scale = { 1.5f, 1.5f };
	float depth = 0.f;

	// 스코어 텍스트 로드
	UINT idx = texts->Load(Text::ID::DEFAULT, _T("점수 : "), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(color);
	texts->GetTextObject(idx)->SetScale(scale);
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
	m_UIIdxList.insert(std::make_pair(ObjectID::SCENE_TEXT_SCORE_TITLE, idx));

	pos.x += 65.f;
	idx = texts->Load(Text::ID::DEFAULT, _T("0"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(pos);
	texts->GetTextObject(idx)->SetColor(color);
	texts->GetTextObject(idx)->SetScale(scale);
	texts->GetTextObject(idx)->SetDepth(depth + 0.01f);
	m_UIIdxList.insert(std::make_pair(ObjectID::SCENE_TEXT_SCORE, idx));
	pos.x -= 65.f;
}

void Scene::SingleModeSceneClass::LoadObject(Object::ObjectManagerClass* objects)
{
	Object::CameraClass* cam = nullptr;

	// 플레이어 로드
	UINT idx = objects->Load(Object::ID::PLAYER, Graphic::Model::ID::NONE);
	objects->SetCameraIdx(idx);
	cam = static_cast<Object::CameraClass*>(objects->GetGameObject(idx));
	cam->SetPosition(DirectX::XMFLOAT4(0.f, 30.f, 0.f, 1.f));
	cam->SetRotation(DirectX::XMFLOAT4(-0.02f, 7.f, 0.f, 1.f));
	cam->SetMoveSpeed(0.15f);
	cam->SetRotateSpeed(0.5f);

	// object 로드
	idx = objects->Load(Object::ID::ACTOR, Graphic::Model::ID::DEFAULT_SPACESHIP);
	objects->GetGameObject(idx)->SetPosition(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));
}

void Scene::SingleModeSceneClass::LimitMouseCursorMoveRange()
{
	RECT rt = {};
	POINT LT = {}, RB = {};

	// 클라이언트 기준의 윈도우 사이즈 가져오기
	GetClientRect(System::hWnd, &rt);

	// 윈도우 사이즈를 좌표로 변환
	LT.x = rt.left;
	LT.y = rt.top;
	RB.x = rt.right;
	RB.y = rt.bottom;

	// 좌표를 스크린 기준으로 변환
	ClientToScreen(System::hWnd, &LT);
	ClientToScreen(System::hWnd, &RB);
	rt.left = LT.x;
	rt.top = LT.y;
	rt.right = RB.x;
	rt.bottom = RB.y;

	// 마우스 움직임 범위가 이전과 다르다면, 현재 범위로 업데이트
	if (rt != m_CurrentMouseCursorMoveRange)
	{
		m_CurrentMouseCursorMoveRange = rt;
		ClipCursor(&m_CurrentMouseCursorMoveRange);
	}
}