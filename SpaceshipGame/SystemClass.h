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
private:
	SystemClass();
	SystemClass(const SystemClass& other);
	~SystemClass();

public:
	static inline SystemClass* const& GetSystemInst()
	{
		if (!inst)
			inst = new SystemClass;

		return inst;
	}

	HRESULT Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	inline const FPSClass* const& GetFPS() const { return m_FPS; }
	inline const CPUClass* const& GetCPU() const { return m_CPU; }
	inline const TimerClass* const& GetTimer() const { return m_Timer; }

private:
	HRESULT Frame();
	void InitializeWindows(const int& ScreenWidth, const int& ScreenHeight);
	void ShutdownWindows();

private:
	static SystemClass* inst;

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