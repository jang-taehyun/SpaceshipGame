#include "pch.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "ActorManagerClass.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(SceneState current) : m_CurrentSceneState(current), m_NextSceneState(SceneState::NONE)
{
	m_TextManager = std::make_unique<Text::TextManagerClass>();
	m_UIManager = std::make_unique<UI::UIManagerClass>();
	m_SoundManager = std::make_unique<Sound::SoundManagerClass>();
	m_ActorManager = std::make_unique<Object::ActorManagerClass>();
	m_Camera = std::make_unique<Object::CameraClass>();
}

Scene::SceneClass::~SceneClass()
{
	// 마우스 커서를 보이게 하고 위치를 중앙으로 되돌리기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}
