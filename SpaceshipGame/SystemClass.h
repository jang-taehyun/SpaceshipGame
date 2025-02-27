#pragma once

class InputClass;
class GraphicsClass;
class SoundClass;
class FPSClass;
class CPUClass;
class TimerClass;
class PositionClass;

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass& other);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindows(int& ScreenWidth, int& ScreenHeight);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName = _T("");
	HINSTANCE m_hinstance = 0;
	HWND m_hwnd = 0;

	InputClass* m_Input = nullptr;
	GraphicsClass* m_Graphics = nullptr;
	SoundClass* m_Sound = nullptr;

	FPSClass* m_FPS = nullptr;
	CPUClass* m_CPU = nullptr;
	TimerClass* m_Timer = nullptr;

	PositionClass* m_Position = nullptr;
};

// 윈도우 종료 또는 닫히는 메세지 처리
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

// 외부 포인터
static SystemClass* ApplicationHandle = nullptr;