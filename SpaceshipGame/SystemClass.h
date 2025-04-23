#pragma once

class InputClass;
class GraphicsClass;
class SoundClass;
class FPSClass;
class CPUClass;
class TimerClass;
class ActorManagerClass;

class SystemClass
{
public:
	explicit SystemClass();
	virtual ~SystemClass();
	
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	inline const FPSClass* const& GetFPS() const { return m_FPS; }
	inline const CPUClass* const& GetCPU() const { return m_CPU; }
	inline const TimerClass* const& GetTimer() const { return m_Timer; }

private:
	HRESULT Initialize();
	void Shutdown();

	HRESULT Frame();
	void InitializeWindows(int& ScreenWidth, int& ScreenHeight);
	void ShutdownWindows();

private:
	static bool IsInitialize;

	LPCWSTR m_applicationName = _T("");
	HINSTANCE m_hinstance = 0;
	HWND m_hwnd = 0;

	InputClass* m_Input = nullptr;
	GraphicsClass* m_Graphics = nullptr;
	SoundClass* m_Sound = nullptr;

	FPSClass* m_FPS = nullptr;
	CPUClass* m_CPU = nullptr;
	TimerClass* m_Timer = nullptr;

	ActorManagerClass* m_ActorManager = nullptr;

public:
	SystemClass(const SystemClass& other) = delete;
};

// 윈도우 종료 또는 닫히는 메세지 처리
static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

// 외부 포인터
static SystemClass* ApplicationHandle = nullptr;