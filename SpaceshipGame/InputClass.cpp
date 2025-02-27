#include "pch.h"
#include "InputClass.h"

InputClass::InputClass() {}
InputClass::InputClass(const InputClass& other) {}
InputClass::~InputClass() {}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int ScreenWidth, int ScreenHeight)
{
	// �ʱ� ���콺 ��ġ ���� //
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;

	HRESULT result;

	// Direct input interface �ʱ�ȭ //
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Ű������ Direct input interface �ʱ�ȭ //
	// Ű������ Direct input interface �ʱ�ȭ
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL)))
	{
		return false;
	}

	// Ű������ Direct input interface�� ������ �������� ����(������ �ؼ� ���) ����
	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	// Ű������ Direct input interface�� ���� Cooperative level ����
	if (FAILED(m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
	{
		return false;
	}

	// Ű������ Direct input�� ���� ���� ���� ���
	if (FAILED(m_Keyboard->Acquire()))
	{
		return false;
	}


	// ���콺�� Direct input interface �ʱ�ȭ //
	if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL)))
	{
		return false;
	}

	// ���콺�� Direct input interface�� ������ �������� ����(������ �ؼ� ���) ����
	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	// ���콺�� Direct input interface�� ���� Cooperative level ����
	if (FAILED(m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	// ���콺�� Direct input�� ���� ���� ���� ���
	if (FAILED(m_Mouse->Acquire()))
	{
		return false;
	}

	return true;
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

bool InputClass::Frame()
{
	if (!ReadKeyboard())
	{
		return false;
	}

	if (!ReadMouse())
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputClass::IsEscapePressed()
{
	if (m_KeyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftArrowPressed()
{
	if (m_KeyboardState[DIK_LEFTARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsRightArrowPressed()
{
	if (m_KeyboardState[DIK_RIGHTARROW] & 0x80)
	{
		return true;
	}

	return false;
}

void InputClass::GetMouseLocation(int& MouseX, int& MouseY)
{
	MouseX = m_MouseX;
	MouseY = m_MouseY;
}

bool InputClass::ReadKeyboard()
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
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
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
			return false;
		}
	}

	return true;
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
