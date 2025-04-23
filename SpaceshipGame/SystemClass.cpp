#include "pch.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TimerClass.h"
#include "ActorManagerClass.h"
#include "SystemClass.h"

bool SystemClass::IsInitialize = false;
static ErrorContent e;

SystemClass::SystemClass()
{	
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SystemClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 SystemClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize();
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}

	IsInitialize = true;
}

SystemClass::~SystemClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT SystemClass::Initialize()
{
	HRESULT result = S_OK;
	int ScreenWidth = WIDTH;
	int ScreenHeight = HEIGHT;

	// 에러 메세지 초기화 //
	e.title = _T("SystemClass Initialize()");

	// 윈도우의 가로, 세로 길이 초기화
	InitializeWindows(ScreenWidth, ScreenHeight);

	// 객체 생성 및 초기화 //
	m_Input = new InputClass(m_hinstance, m_hwnd, ScreenWidth, ScreenHeight);
	if (!m_Input)
	{
		e.contents = _T("InputClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_Graphics = new GraphicsClass(ScreenWidth, ScreenHeight, m_hwnd);
	if (!m_Graphics)
	{
		e.contents = _T("GraphicsClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_Sound = new SoundClass();
	if (!m_Sound)
	{
		e.contents = _T("SoundClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_FPS = new FPSClass;
	if (!m_FPS)
	{
		e.contents = _T("FPSClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_CPU = new CPUClass;
	if (!m_CPU)
	{
		e.contents = _T("CPUClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		e.contents = _T("TimerClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_ActorManager = new ActorManagerClass;
	if (!m_ActorManager)
	{
		e.contents = _T("ActorManagerClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void SystemClass::Shutdown()
{
	if (m_ActorManager)
	{
		delete m_ActorManager;
		m_ActorManager = nullptr;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}

	if (m_CPU)
	{
		delete m_CPU;
		m_CPU = nullptr;
	}

	if (m_FPS)
	{
		delete m_FPS;
		m_FPS = nullptr;
	}

	if (m_Sound)
	{
		delete m_Sound;
		m_Sound = nullptr;
	}

	if (m_Graphics)
	{
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = nullptr;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	HRESULT result = S_OK;
	MSG msg;

	// 에러 메세지 초기화 //
	e.title = _T("SystemClass Run()");

	// 메세지 구조체 초기화 //
	memset(&msg, 0, sizeof(MSG));

	while (1)
	{
		// 윈도우 메세지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// frame 처리
		else
		{
			result = Frame();
			if (FAILED(result))
			{
				e.contents = _T("Frame() 처리 실패");
				e.errorCode = result;
				return;
			}
		}

		if (m_Input->IsEscapePressed())
			break;
	}
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

HRESULT SystemClass::Frame()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SystemClass Frame()");

	// Timer, FPS, CPU, Input, Sound의 Frame() 진행 //
	m_Timer->Frame();
	m_FPS->Frame();
	m_CPU->Frame();

	result = m_Input->Frame();
	if (FAILED(result))
		return result;

	result = m_Sound->Frame();
	if (FAILED(result))
		return result;

	// Graphics의 Frame() 진행 //
	result = m_Graphics->Frame(m_ActorManager, m_Sound, m_Input, m_Timer->GetTime(), m_FPS->GetFPS(), (int)m_CPU->GetCPUPercentage());
	if (FAILED(result))
		return result;

	return result;
}

void SystemClass::InitializeWindows(int& ScreenWidth, int& ScreenHeight)
{
	int width = 0, height = 0, PosX = 0, PosY = 0;

	// 외부 pointer를 현재 instance를 가르키도록 한다.
	ApplicationHandle = this;

	// 현재 프로그램의 instance를 가져오기
	m_hinstance = GetModuleHandle(NULL);

	// windows 클래스 정보 설정 및 등록
	m_applicationName = _T("SpaceshipGame");

	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	if (FULL_SCREEN)
	{
		// 모니터의 해상도 가져오기
		ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		width = ScreenWidth;
		height = ScreenHeight;

		// 풀스크린 모드 //
		// 모니터 화면 해상도를 desktop 해상도로 지정
		// 색상 : 32bit
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		// 윈도우 모드 //
		// 모니터 화면 해상도를 800*600으로 지정
		// 윈도우의 위치 : 정가운데
		ScreenWidth = WIDTH;
		ScreenHeight = HEIGHT;
		width = ScreenWidth;
		height = ScreenHeight;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// 윈도우 생성 및 handle 가지오기
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		PosX, PosY, width, height, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 focus를 지정
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서 위치를 중앙으로 초기화 //
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}

void SystemClass::ShutdownWindows()
{
	// 풀스크린 모드라면, 디스플레이 설정을 초기화
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	// 윈도우 제거
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 프로그램의 instance 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 외부 pointer 초기화
	ApplicationHandle = NULL;
}

// IMGUI WndProcHandler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return 0;

	switch (umessage)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
