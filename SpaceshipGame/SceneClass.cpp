#include "pch.h"
#include "CameraClass.h"
#include "SceneClass.h"

Scene::SceneClass::SceneClass(ID current, ID next)
	: m_CurrentSceneState(current),
	m_NextSceneState(next)
{}

Scene::SceneClass::~SceneClass()
{
	// 마우스 커서를 보이게 하기 //
	if (!m_IsShowingCursor)
		ShowCursor(true);
}

LRESULT Scene::SceneClass::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umessage, wparam, lparam);
}

static LRESULT CALLBACK Scene::SceneWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (SceneHandler)
		return reinterpret_cast<ISceneClass*>(SceneHandler)->MessageHandler(hwnd, umessage, wparam, lparam);

	return DefWindowProc(hwnd, umessage, wparam, lparam);
}
