#include "pch.h"
#include "InputClass.h"

bool System::InputClass::IsInitailize = false;

static const UINT OriginKey[static_cast<UINT>(System::KEY::LAST)] =
{
	DIK_W,					// W
	DIK_A,					// A
	DIK_S,					// S
	DIK_D,					// D
	
	DIK_UPARROW,			// UP_ARROW
	DIK_LEFTARROW,			// LEFT_ARROW
	DIK_DOWNARROW,			// DOWN_ARROW
	DIK_RIGHTARROW,			// RIGHT_ARROW
	
	DIK_ESCAPE,				// ESC
	DIK_SPACE,				// SPACEBAR
	
	0,						// MOUSE_LEFT
	1,						// MOUSE_RIGHT
	2,						// MOUSE_CENTER
	
	
	// LAST
};

System::InputClass::InputClass(int ScreenWidth, int ScreenHeight)
{
	HRESULT result = S_OK;
	
	assert(!IsInitailize);

	result = Initialize(ScreenWidth, ScreenHeight);
	IsInitailize = true;
}

System::InputClass::~InputClass()
{
	Shutdown();
	IsInitailize = false;
}

HRESULT System::InputClass::Initialize(int ScreenWidth, int ScreenHeight)
{
	HRESULT result = S_OK;

	// Direct input interface 초기화 //
	result = DirectInput8Create(
		hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		reinterpret_cast<void**>(m_DirectInput.GetAddressOf()),
		NULL);
	assert(SUCCEEDED(result));

	// 키보드의 Direct input interface 초기화 //
	// 키보드의 Direct input interface 초기화
	result = m_DirectInput->CreateDevice(GUID_SysKeyboard, m_Keyboard.GetAddressOf(), NULL);
	assert(SUCCEEDED(result));

	// 키보드의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정
	result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	// 키보드의 Direct input interface에 대한 Cooperative level 설정
	result = m_Keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	assert(SUCCEEDED(result));

	// 키보드의 Direct input에 대한 접근 권한 취득
	result = m_Keyboard->Acquire();
	assert(SUCCEEDED(result));


	// 마우스의 Direct input interface 초기화 //
	result = m_DirectInput->CreateDevice(GUID_SysMouse, m_Mouse.GetAddressOf(), NULL);
	assert(SUCCEEDED(result));

	// 마우스의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정
	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	// 마우스의 Direct input interface에 대한 Cooperative level 설정
	result = m_Mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// 마우스의 Direct input에 대한 접근 권한 취득
	result = m_Mouse->Acquire();
	assert(SUCCEEDED(result));

	return result;
}

void System::InputClass::Shutdown()
{
	if (m_Mouse)
		m_Mouse->Unacquire();

	if (m_Keyboard)
		m_Keyboard->Unacquire();
}

void System::InputClass::Frame()
{
	HRESULT result = S_OK;

	// 키보드의 상태 읽기 //
	result = ReadKeyboard();

	// 마우스의 상태 읽기 //
	result = ReadMouse();

	ProcessInput();
}

System::KEYSTATE System::InputClass::GetKeyState(KEY key) const
{
	assert(static_cast<UINT>(KEY::LAST) > static_cast<UINT>(key));
	return m_State[static_cast<UINT>(key)].CurrentState;
}

HRESULT System::InputClass::ReadKeyboard()
{
	HRESULT result = S_OK;

	// 키보드의 현재 상태 가져오기 //
	result = m_Keyboard->GetDeviceState(
		sizeof(m_KeyboardState),
		reinterpret_cast<LPVOID>(&m_KeyboardState)
	);
	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
		{
			m_Keyboard->Acquire();
		}
		else
			assert(SUCCEEDED(result));
	}

	return result;
}

HRESULT System::InputClass::ReadMouse()
{
	HRESULT result = S_OK;

	// 마우스의 상태 가져오기 //
	result = m_Mouse->GetDeviceState(
		sizeof(m_MouseState),
		reinterpret_cast<LPVOID>(&m_MouseState)
	);
	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
		{
			m_Mouse->Acquire();
		}
		else
			assert(SUCCEEDED(result));
	}

	return result;
}

void System::InputClass::ProcessInput()
{
	UINT cnt = static_cast<UINT>(KEY::LAST);
	KEY key = KEY::LAST;

	// 키 상태 업데이트
	for (UINT i = 0; i < cnt; ++i)
	{
		key = static_cast<KEY>(i);

		// mouse button 업데이트
		if (KEY::MOUSE_LEFT == key || KEY::MOUSE_CENTER == key || KEY::MOUSE_RIGHT == key)
			UpdateMouseButton(i);

		// keyboard button 업데이트
		else
			UpdateKeyboardButton(i);
	}

	// 화면 상의 마우스 좌표 가져오기
	GetCursorPos(&m_MousePos);

	// 마우스 좌표를 client size 기준으로 변환
	ScreenToClient(System::hWnd, &m_MousePos);
}

void System::InputClass::UpdateMouseButton(UINT idx)
{
	// 키가 눌렸을 때
	if (m_MouseState.rgbButtons[OriginKey[idx]] & 0x80)
	{
		// 이전에도 키를 눌렀을 때
		if (m_State[idx].PreviousPush)
			m_State[idx].CurrentState = KEYSTATE::HOLD;

		// 이전에는 키를 누르지 않았을 때
		else
			m_State[idx].CurrentState = KEYSTATE::TAP;

		m_State[idx].PreviousPush = true;
	}

	// 키가 안눌렸을 때
	else
	{
		// 이전에 키를 눌렀을 때
		if (m_State[idx].PreviousPush)
			m_State[idx].CurrentState = KEYSTATE::AWAY;

		// 이전에는 키를 누르지 않았을 때
		else
			m_State[idx].CurrentState = KEYSTATE::NONE;

		m_State[idx].PreviousPush = false;
	}
}

void System::InputClass::UpdateKeyboardButton(UINT idx)
{
	// 키가 눌렸을 때
	if (m_KeyboardState[OriginKey[idx]] & 0x80)
	{
		// 이전에도 키를 눌렀을 때
		if (m_State[idx].PreviousPush)
			m_State[idx].CurrentState = KEYSTATE::HOLD;

		// 이전에는 키를 누르지 않았을 때
		else
			m_State[idx].CurrentState = KEYSTATE::TAP;

		m_State[idx].PreviousPush = true;
	}

	// 키가 안눌렸을 때
	else
	{
		// 이전에 키를 눌렀을 때
		if (m_State[idx].PreviousPush)
			m_State[idx].CurrentState = KEYSTATE::AWAY;

		// 이전에는 키를 누르지 않았을 때
		else
			m_State[idx].CurrentState = KEYSTATE::NONE;

		m_State[idx].PreviousPush = false;
	}
}
