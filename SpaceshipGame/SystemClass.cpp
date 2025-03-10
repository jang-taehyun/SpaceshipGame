#include "pch.h"
#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TimerClass.h"
#include "PositionClass.h"
#include "SystemClass.h"

SystemClass::SystemClass() {}
SystemClass::SystemClass(const SystemClass& other) {}
SystemClass::~SystemClass() {}

HRESULT SystemClass::Initialize()
{
	// �������� ����, ���� ���� �ʱ�ȭ
	int ScreenWidth = WIDTH;
	int ScreenHeight = HEIGHT;
	InitializeWindows(ScreenWidth, ScreenHeight);

	// ��ü ���� �� �ʱ�ȭ
	m_Input = new InputClass;
	if (!m_Input)
		return E_FAIL;
	if (FAILED(m_Input->Initialize(m_hinstance, m_hwnd, ScreenWidth, ScreenHeight)))
	{
		return E_FAIL;
	}

	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
		return E_FAIL;

	if (FAILED(m_Graphics->Initialize(ScreenWidth, ScreenHeight, m_hwnd)))
	{
		return E_FAIL;
	}

	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return E_FAIL;
	}

	if (FAILED(m_Sound->Initialize(m_hwnd, s_info)))
	{
		return E_FAIL;
	}

	m_FPS = new FPSClass;
	if (!m_FPS)
	{
		return E_FAIL;
	}
	m_FPS->Initialize();

	m_CPU = new CPUClass;
	if (!m_CPU)
	{
		return E_FAIL;
	}
	m_CPU->Intialize();

	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return E_FAIL;
	}
	if (FAILED(m_Timer->Initialize()))
	{
		return E_FAIL;
	}

	m_Position = new PositionClass;
	if (!m_Position)
	{
		return E_FAIL;
	}

	return S_OK;
}

void SystemClass::Shutdown()
{
	if (m_Position)
	{
		delete m_Position;
		m_Position = nullptr;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}

	if (m_CPU)
	{
		m_CPU->Shutdown();
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
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = nullptr;
	}

	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	if (m_Input)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = nullptr;
	}

	ShutdownWindows();
}

void SystemClass::Run()
{
	// �޼��� ����ü ���� �� �ʱ�ȭ
	MSG msg;
	memset(&msg, 0, sizeof(MSG));

	while (1)
	{
		// ������ �޼��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// frame ó��
		else
		{
			if (FAILED(Frame()))
				break;
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
	int MouseX, MouseY;

	m_Timer->Frame();
	m_FPS->Frame();
	m_CPU->Frame();

	if (FAILED(m_Input->Frame()))
		return E_FAIL;

	m_Input->GetMouseLocation(MouseX, MouseY);

	float rotationY = 0.f;
	bool KeyDown = false;

	// position ��ü�� frame time ���� �� Ű���� ���� Ȯ�� //
	m_Position->SetFrameTime(m_Timer->GetTime());

	KeyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(KeyDown);

	KeyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(KeyDown);

	// camera�� ȸ������ �����Ͽ� ���� ī�޸� ��ġ �ݿ� //
	rotationY = m_Position->GetRotation();

	return m_Graphics->Frame(rotationY, m_hwnd);
}

void SystemClass::InitializeWindows(const int& ScreenWidth, const int& ScreenHeight)
{
	int width = ScreenWidth;
	int height = ScreenHeight;

	// �ܺ� pointer�� ���� instance�� ����Ű���� �Ѵ�.
	ApplicationHandle = this;

	// ���� ���α׷��� instance�� ��������
	m_hinstance = GetModuleHandle(NULL);

	// windows Ŭ���� ���� ���� �� ���
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

	// ������� �ػ� ��������
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);

	int PosX = 0, PosY = 0;

	if (FULL_SCREEN)
	{
		// Ǯ��ũ�� ��� //
		// ����� ȭ�� �ػ󵵸� desktop �ػ󵵷� ����
		// ���� : 32bit
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight = (unsigned long)width;
		dmScreenSettings.dmPelsWidth = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		// ������ ��� //
		// ����� ȭ�� �ػ󵵸� 800*600���� ����
		// �������� ��ġ : �����
		width = 800;
		height = 600;

		PosX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		PosY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	// ������ ���� �� handle ��������
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		PosX, PosY, width, height, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� focus�� ����
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}

void SystemClass::ShutdownWindows()
{
	// Ǯ��ũ�� �����, ���÷��� ������ �ʱ�ȭ
	if (FULL_SCREEN)
		ChangeDisplaySettings(NULL, 0);

	// ������ ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���α׷��� instance ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �ܺ� pointer �ʱ�ȭ
	ApplicationHandle = NULL;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
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
