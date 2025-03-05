#include "pch.h"
#include "InputClass.h"

InputClass::InputClass() {}
InputClass::InputClass(const InputClass& other) {}
InputClass::~InputClass() {}

HRESULT InputClass::Initialize(const HINSTANCE& hinstance, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight)
{
	// 초기 마우스 위치 설정 //
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;

	HRESULT result;

	// Direct input interface 초기화 //
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);
	if (FAILED(result))
	{
		return E_FAIL;
	}

	// 키보드의 Direct input interface 초기화 //
	// 키보드의 Direct input interface 초기화
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL)))
	{
		return E_FAIL;
	}

	// 키보드의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정
	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 키보드의 Direct input interface에 대한 Cooperative level 설정
	if (FAILED(m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		return E_FAIL;
	}

	// 키보드의 Direct input에 대한 접근 권한 취득
	if (FAILED(m_Keyboard->Acquire()))
	{
		return E_FAIL;
	}


	// 마우스의 Direct input interface 초기화 //
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL)))
	{
		return E_FAIL;
	}

	// 마우스의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정
	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 마우스의 Direct input interface에 대한 Cooperative level 설정
	if (FAILED(m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return E_FAIL;
	}

	// 마우스의 Direct input에 대한 접근 권한 취득
	if (FAILED(m_Mouse->Acquire()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void InputClass::Shutdown()
{
	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = nullptr;
	}

	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard = nullptr;
	}

	if (m_DirectInput)
	{
		m_DirectInput->Release();
		m_DirectInput = nullptr;
	}
}

HRESULT InputClass::Frame()
{
	if (FAILED(ReadKeyboard()))
	{
		return E_FAIL;
	}

	if (FAILED(ReadMouse()))
	{
		return E_FAIL;
	}

	ProcessInput();

	return S_OK;
}

HRESULT InputClass::ReadKeyboard()
{
	HRESULT result;

	result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
		{
			m_Keyboard->Acquire();
		}
		else
		{
			return result;
		}
	}

	return S_OK;
}

HRESULT InputClass::ReadMouse()
{
	HRESULT result;

	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);
	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
		{
			m_Mouse->Acquire();
		}
		else
		{
			return result;
		}
	}

	return S_OK;
}

void InputClass::ProcessInput()
{
	m_MouseX += m_MouseState.lX;
	m_MouseY += m_MouseState.lY;

	if (m_MouseX < 0)
	{
		m_MouseX = 0;
	}
	if (m_MouseY < 0)
	{
		m_MouseY = 0;
	}
}
