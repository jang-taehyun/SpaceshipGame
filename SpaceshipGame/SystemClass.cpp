#include "pch.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TimerClass.h"
#include "SceneManagerClass.h"
#include "SystemClass.h"

bool System::SystemClass::IsInitialize = false;

System::SystemClass::SystemClass()
{	
	assert(!IsInitialize);

	Initialize();
	IsInitialize = true;
}

System::SystemClass::~SystemClass()
{
	ShutdownWindows();
	IsInitialize = false;
}

void System::SystemClass::Initialize()
{
	UINT ScreenWidth = System::RESOLUTIONS[System::CURRENT_RESOLUTION_INDEX].WIDTH;
	UINT ScreenHeight = System::RESOLUTIONS[System::CURRENT_RESOLUTION_INDEX].HEIGHT;

	// 윈도우의 가로, 세로 길이 초기화
	InitializeWindows(ScreenWidth, ScreenHeight);

	// 객체 생성 및 초기화 //
	m_Input = std::make_unique<InputClass>(ScreenWidth, ScreenHeight);
	assert(m_Input);

	m_Graphics = std::make_unique<Graphic::GraphicsClass>(ScreenWidth, ScreenHeight);
	assert(m_Graphics);

	m_FPS = std::make_unique<FPSClass>();
	assert(m_FPS);

	m_CPU = std::make_unique<CPUClass>();
	assert(m_CPU);

	m_Timer = std::make_unique<TimerClass>();
	assert(m_Timer);

	m_SceneManager = std::make_unique<Scene::SceneManagerClass>();
	assert(m_SceneManager);
}

void System::SystemClass::Run()
{
	MSG msg;

	// 메세지 구조체 초기화 //
	memset(&msg, 0, sizeof(MSG));

	while (System::RUNNING)
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
			Frame();

		if (m_Input->IsEscapePressed())
			break;
	}
}

LRESULT System::SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (Scene::SceneHandler)
		return reinterpret_cast<Scene::ISceneClass*>(Scene::SceneHandler)->MessageHandler(hwnd, umsg, wparam, lparam);

	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void System::SystemClass::Frame()
{
	bool IsLoad = false;

	m_Timer->Frame();
	m_FPS->Frame();
	m_CPU->Frame();

	m_Input->Frame();

	IsLoad = m_SceneManager->Frame(m_Input.get(), m_Timer->GetTime());
	m_Graphics->Frame(m_SceneManager.get(), IsLoad
#ifdef _DEBUG
		, m_Input.get()
#endif // _DEBUG
	);
}

void System::SystemClass::InitializeWindows(UINT& ScreenWidth, UINT& ScreenHeight)
{
	int width = 0, height = 0, PosX = 0, PosY = 0;
	RECT rc = {};

	// 외부 pointer를 현재 instance를 가르키도록 한다.
	ApplicationHandle = this;

	// 현재 프로그램의 instance를 가져오기
	hInst = GetModuleHandle(NULL);

	// windows 클래스 정보 설정 및 등록
	m_applicationName = _T("SpaceshipGame");

	WNDCLASSEX wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SPACESHIPGAME));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SMALL));
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
		// 모니터 화면 해상도를 1280*800으로 지정
		// 윈도우의 위치 : 정가운데
		ScreenWidth = System::RESOLUTIONS[System::CURRENT_RESOLUTION_INDEX].WIDTH;
		ScreenHeight = System::RESOLUTIONS[System::CURRENT_RESOLUTION_INDEX].HEIGHT;
		width = ScreenWidth;
		height = ScreenHeight;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// 윈도우 생성 및 handle 가지오기
	hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		PosX, PosY, width, height, NULL, NULL, hInst, NULL);

	// width, height를 client size로 변경
	if (!FULL_SCREEN)
	{
		GetClientRect(hWnd, &rc);
		ScreenWidth = rc.right - rc.left;
		ScreenHeight = rc.bottom - rc.top;
	}

	// 윈도우를 화면에 표시하고 focus를 지정
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	// 마우스 커서 위치를 중앙으로 초기화 //
	SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
}

void System::SystemClass::ShutdownWindows()
{
	// 풀스크린 모드라면, 디스플레이 설정을 초기화
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	// 윈도우 제거
	DestroyWindow(hWnd);
	hWnd = NULL;

	// 프로그램의 instance 제거
	UnregisterClass(m_applicationName, hInst);
	hInst = NULL;

	// 외부 pointer 초기화
	ApplicationHandle = NULL;
}

#ifdef _DEBUG
// ImGui WndProcHandler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
#endif

static LRESULT CALLBACK System::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
#ifdef _DEBUG
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
		return 1;
#endif

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
		return System::ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}
