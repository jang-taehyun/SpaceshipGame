#include "pch.h"

#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"

#include "IUIClass.h"
#include "ButtonClass.h"

#include "ITextClass.h"

#include "ModeChooseSceneClass.h"

Scene::ModeChooseSceneClass::ModeChooseSceneClass(ID next, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds)
	: SceneClass(ID::MODE_CHOOSE, next, texts, UIs)
{
	LoadESCPopupWindow(texts, UIs);
	LoadSound(sounds);
	Load2DBackground(UIs);
	LoadSingleModeButton(texts, UIs);
	LoadMultiModeButton(texts, UIs);
}

Scene::ModeChooseSceneClass::ModeChooseSceneClass(const ModeChooseSceneClass& other)
	: SceneClass(other)
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::ModeChooseSceneClass::ModeChooseSceneClass(ModeChooseSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_ObjectList(std::move(other.m_ObjectList))
{}

Scene::ModeChooseSceneClass& Scene::ModeChooseSceneClass::operator=(const ModeChooseSceneClass& other)
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	SceneClass::operator=(other);

	return *this;
}

Scene::ModeChooseSceneClass& Scene::ModeChooseSceneClass::operator=(ModeChooseSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ObjectList.clear();

	m_ObjectList = std::move(other.m_ObjectList);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::ModeChooseSceneClass::ActiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	std::map<ObjectID, UINT>::iterator iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
			// scene의 UI 활성화
			case ObjectID::SCENE_BACKGROUND:
			case ObjectID::SCENE_BUTTON_UI_SINGLEMODE:
			case ObjectID::SCENE_BUTTON_UI_MULTIMODE:
			{
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, true);
				break;
			}

			// scene의 text 활성화
			case ObjectID::SCENE_BUTTON_TEXT_SINGLEMODE:
			case ObjectID::SCENE_BUTTON_TEXT_MULTIMODE:
			{
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, true);
				break;
			}
		}
	}
}

void Scene::ModeChooseSceneClass::DeactiveChildSceneUI(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	std::map<ObjectID, UINT>::iterator iter = m_ObjectList.begin();
	for (; iter != m_ObjectList.end(); ++iter)
	{
		switch (iter->first)
		{
			// scene의 UI 활성화
			case ObjectID::SCENE_BACKGROUND:
			case ObjectID::SCENE_BUTTON_UI_SINGLEMODE:
			case ObjectID::SCENE_BUTTON_UI_MULTIMODE:
			{
				UIs->GetUI(iter->second)->SetUIState(UI::UIState::ACTIVE, false);
				break;
			}

			// scene의 text 활성화
			case ObjectID::SCENE_BUTTON_TEXT_SINGLEMODE:
			case ObjectID::SCENE_BUTTON_TEXT_MULTIMODE:
			{
				texts->GetTextObject(iter->second)->SetTextState(UI::UIState::ACTIVE, false);
				break;
			}
		}
	}
}

void Scene::ModeChooseSceneClass::ProcessChildScene(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	if (ProcessSingleModeButton(UIs->GetUI(m_ObjectList.find(ObjectID::SCENE_BUTTON_UI_SINGLEMODE)->second)))
		return;

	if (ProcessMultiModeButton(UIs->GetUI(m_ObjectList.find(ObjectID::SCENE_BUTTON_UI_MULTIMODE)->second)))
		return;
}

void Scene::ModeChooseSceneClass::LoadSound(Sound::SoundManagerClass* sounds)
{
	UINT SoundMask = 0;

	// sound 로드 및 재생
	SoundMask |= (1 << static_cast<UINT>(Sound::ID::BACKGROUND));
	sounds->Load(SoundMask);
	sounds->SetLoop(Sound::ID::BACKGROUND, true);
	sounds->Play(Sound::ID::BACKGROUND);
}

void Scene::ModeChooseSceneClass::Load2DBackground(UI::UIManagerClass* UIs)
{
	float depth = 0.f;

	// 백그라운드 로드
	UINT idx = UIs->LoadUI(UI::ID::BACKGROUND, Graphic::Texture::UITextureID::START_BACKGROUND);
	UIs->GetUI(idx)->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(0.25f, 0.25f));
	UIs->GetUI(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BACKGROUND, idx));
}

void Scene::ModeChooseSceneClass::LoadSingleModeButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	float depth = 0.1f;
	DirectX::XMFLOAT4 textColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	// 싱글 플레이 모드 버튼 생성 //
	UINT idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	UIs->GetUI(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BUTTON_UI_SINGLEMODE, idx));

	idx = texts->Load(Text::ID::DEFAULT, _T("싱글 플레이"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(DirectX::XMFLOAT2(600.f, 550.f));
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BUTTON_TEXT_SINGLEMODE, idx));
}

void Scene::ModeChooseSceneClass::LoadMultiModeButton(Text::TextManagerClass* texts, UI::UIManagerClass* UIs)
{
	float depth = 0.1f;
	DirectX::XMFLOAT4 textColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);

	// 멀티 플레이 모드 버튼 생성 //
	UINT idx = UIs->LoadUI(UI::ID::BUTTON, Graphic::Texture::UITextureID::START_BUTTON);
	UIs->GetUI(idx)->SetPosition(DirectX::XMFLOAT2(600.f, 350.f));
	UIs->GetUI(idx)->SetScale(DirectX::XMFLOAT2(800.f, 150.f));
	UIs->GetUI(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BUTTON_UI_MULTIMODE, idx));

	idx = texts->Load(Text::ID::DEFAULT, _T("멀티 플레이(활성화 되지 않음)"), Graphic::Font::ID::DEFAULT);
	texts->GetTextObject(idx)->SetPosition(DirectX::XMFLOAT2(600.f, 350.f));
	texts->GetTextObject(idx)->SetColor(textColor);
	texts->GetTextObject(idx)->SetScale(DirectX::XMFLOAT2(1.f, 1.f));
	texts->GetTextObject(idx)->SetDepth(depth);
	m_ObjectList.insert(std::make_pair(ObjectID::SCENE_BUTTON_TEXT_MULTIMODE, idx));
}

bool Scene::ModeChooseSceneClass::ProcessSingleModeButton(UI::IUIClass* button)
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
	{
		if (ID::SINGLE_MODE != GetNextSceneID())
			SetNextScene(ID::SINGLE_MODE);

		SetSceneEnded();
		return true;
	}

	return false;
}

bool Scene::ModeChooseSceneClass::ProcessMultiModeButton(UI::IUIClass* button)
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
	{
		// ------ 활성화가 된 경우, 해당 코드 삭제 ------ //
		{
			MessageBox(GetActiveWindow(), _T("멀티플레이 모드가 활성화되지 않았습니다."), _T("경고"), MB_OK);
			return true;
		}
		// -------------------------------------------- //

		if (ID::MULTI_MODE != GetNextSceneID())
			SetNextScene(ID::MULTI_MODE);

		SetSceneEnded();
		return true;
	}

	return false;
}
