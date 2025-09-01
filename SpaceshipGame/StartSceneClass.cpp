#include "pch.h"

#pragma comment(lib, "uxtheme.lib")
#include <Uxtheme.h>

#include "ObjectManagerClass.h"
#include "TextManagerClass.h"
#include "UIManagerClass.h"
#include "SoundManagerClass.h"
#include "InputClass.h"

#include "ActorClass.h"

#include "IUIClass.h"
// #include "ButtonClass.h"

// #include "ITextClass.h"

#include "SystemClass.h"

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

	SceneHandler = reinterpret_cast<void*>(this);

	// 시작 버튼 로드
	m_hStartButton = CreateWindow(
		L"BUTTON",
		L"START",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP | BS_FLAT | BS_TEXT | BS_VCENTER | BS_MULTILINE,
		500,
		500,
		100,
		100,
		System::hWnd,
		reinterpret_cast<HMENU>(ObjectID::UI_START_BUTTON),
		System::hInst,
		NULL
	);

	HBITMAP hStartButtonBitmap = (HBITMAP)LoadImage(System::hInst, MAKEINTRESOURCE(IDB_START_BUTTON), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	SendMessage(m_hStartButton, BM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(hStartButtonBitmap));
}

Scene::StartSceneClass::StartSceneClass(const StartSceneClass& other)
	: SceneClass(other),
	m_hStartButton(other.m_hStartButton)
{
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
}

Scene::StartSceneClass::StartSceneClass(StartSceneClass&& other) noexcept
	: SceneClass(std::move(other)),
	m_ObjectList(std::move(other.m_ObjectList)),
	m_hStartButton(other.m_hStartButton)
{}

Scene::StartSceneClass::~StartSceneClass()
{
	DestroyWindow(m_hStartButton);
	SceneHandler = nullptr;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(const StartSceneClass& other)
{
	if (this == &other)
		return *this;

	if(m_hStartButton)
		DestroyWindow(m_hStartButton);

	m_hStartButton = other.m_hStartButton;
	m_ObjectList.insert(other.m_ObjectList.begin(), other.m_ObjectList.end());
	SceneClass::operator=(other);

	return *this;
}

Scene::StartSceneClass& Scene::StartSceneClass::operator=(StartSceneClass&& other) noexcept
{
	if (this == &other)
		return *this;

	if (m_hStartButton)
		DestroyWindow(m_hStartButton);

	m_hStartButton = other.m_hStartButton;
	m_ObjectList = std::move(other.m_ObjectList);
	SceneClass::operator=(std::move(other));

	return *this;
}

void Scene::StartSceneClass::Frame(const System::InputClass* input, Object::ObjectManagerClass* objects, Text::TextManagerClass* texts, UI::UIManagerClass* UIs, Sound::SoundManagerClass* sounds, float frame_time)
{
	
}

LRESULT CALLBACK Scene::StartSceneClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case static_cast<int>(ObjectID::UI_START_BUTTON):
		{
			switch (HIWORD(wparam))
			{
			case BN_CLICKED:
			{
				SetSceneEnded();
				return 1;
			}
			default:
				return DefWindowProc(hwnd, umsg, wparam, lparam);
			}
		}
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}