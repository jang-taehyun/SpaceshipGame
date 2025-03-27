#include "pch.h"
#include "InputClass.h"

bool InputClass::IsInitailize = false;

InputClass::InputClass(const HINSTANCE& hinstance, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("InputClass Constructor");
	
	if (IsInitailize)
	{
		e.contents = _T("이미 InputClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;

		throw e;
	}

	result = Initialize(hinstance, hwnd, ScreenWidth, ScreenHeight);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("InputClass 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	IsInitailize = true;
}

InputClass::~InputClass()
{
	Shutdown();
	IsInitailize = false;
}

HRESULT InputClass::Initialize(const HINSTANCE& hinstance, const HWND& hwnd, const int& ScreenWidth, const int& ScreenHeight)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("InputClass Initailize()");

	// 초기 마우스 위치 설정 //
	m_ScreenHeight = ScreenHeight;
	m_ScreenWidth = ScreenWidth;

	// Direct input interface 초기화 //
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);
	if (FAILED(result))
	{
		e.contents = _T("Direct input interface 생성 실패");
		e.errorCode = result;
		throw e;
	}

	// 키보드의 Direct input interface 초기화 //
	// 키보드의 Direct input interface 초기화
	result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);
	if (FAILED(result))
	{
		e.contents = _T("키보드의 Direct input interface 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	// 키보드의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정
	result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		e.contents = _T("키보드의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정 실패");
		e.errorCode = result;
		throw e;
	}

	// 키보드의 Direct input interface에 대한 Cooperative level 설정
	result = m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		e.contents = _T("키보드의 Direct input interface에 대한 Cooperative level 설정 실패");
		e.errorCode = result;
		throw e;
	}

	// 키보드의 Direct input에 대한 접근 권한 취득
	result = m_Keyboard->Acquire();
	if (FAILED(result))
	{
		e.contents = _T("키보드의 Direct input에 대한 접근 권한 취득 실패");
		e.errorCode = result;
		throw e;
	}


	// 마우스의 Direct input interface 초기화 //
	result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	if (FAILED(result))
	{
		e.contents = _T("마우스의 Direct input interface 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	// 마우스의 Direct input interface가 수집할 데이터의 포맷(데이터 해석 방법) 설정
	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		e.contents = _T("마우스의 Direct input interface 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	// 마우스의 Direct input interface에 대한 Cooperative level 설정
	result = m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		e.contents = _T("마우스의 Direct input interface에 대한 Cooperative level 설정 실패");
		e.errorCode = result;
		throw e;
	}

	// 마우스의 Direct input에 대한 접근 권한 취득
	result = m_Mouse->Acquire();
	if (FAILED(result))
	{
		e.contents = _T("마우스의 Direct input에 대한 접근 권한 취득 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
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
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("InputClass Frame()");

	// 키보드의 상태 읽기 //
	result = ReadKeyboard();
	if (FAILED(result))
	{
		e.contents = _T("키보드의 상태 읽기 실패");
		e.errorCode = result;
		throw e;
	}

	// 마우스의 상태 읽기 //
	result = ReadMouse();
	if (FAILED(result))
	{
		e.contents = _T("마우스의 상태 읽기 실패");
		e.errorCode = result;
		throw e;
	}

	ProcessInput();

	return result;
}

HRESULT InputClass::ReadKeyboard()
{
	ErrorContent e;
	HRESULT result;

	// 에러 메세지 초기화 //
	e.title = _T("InputClass ReadKeyboard()");

	// 키보드의 상태 가져오기 //
	result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
		{
			m_Keyboard->Acquire();
		}
		else
		{
			e.contents = _T("키보드의 상태 가져오기 실패");
			e.errorCode = result;
			throw e;
		}
	}

	return S_OK;
}

HRESULT InputClass::ReadMouse()
{
	HRESULT result;
	ErrorContent e;

	// 에러 메세지 초기화 //
	e.title = _T("InputClass ReadMouse()");

	// 마우스의 상태 가져오기 //
	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), (LPVOID)&m_MouseState);
	if (FAILED(result))
	{
		if (DIERR_INPUTLOST == result || DIERR_NOTACQUIRED == result)
		{
			m_Mouse->Acquire();
		}
		else
		{
			e.contents = _T("마우스의 상태 가져오기 실패");
			e.errorCode = result;
			throw e;
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
