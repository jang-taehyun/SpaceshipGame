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

// ������ ���� �Ǵ� ������ �޼��� ó��
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

// �ܺ� ������
static SystemClass* ApplicationHandle = nullptr;