#pragma once

class InputClass;
class GraphicsClass;
class SoundClass;
class FPSClass;
class CPUClass;
class TimerClass;
class ActorManagerClass;
class ProcessManagerClass;

namespace System
{
	class SystemClass
	{
	public:
		SystemClass();
		virtual ~SystemClass();

		void Run();

		LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

		inline const FPSClass* GetFPS() const { return m_FPS.get(); }
		inline const CPUClass* GetCPU() const { return m_CPU.get(); }
		inline const TimerClass* GetTimer() const { return m_Timer.get(); }

	private:
		HRESULT Initialize();

		HRESULT Frame();
		void InitializeWindows(int& ScreenWidth, int& ScreenHeight);
		void ShutdownWindows();

	private:
		static bool IsInitialize;

		LPCWSTR m_applicationName = _T("");
		HINSTANCE m_hinstance = 0;
		HWND m_hwnd = 0;

		std::unique_ptr<InputClass> m_Input = nullptr;
		std::unique_ptr<GraphicsClass> m_Graphics = nullptr;
		std::unique_ptr<SoundClass> m_Sound = nullptr;

		std::unique_ptr<FPSClass> m_FPS = nullptr;
		std::unique_ptr<CPUClass> m_CPU = nullptr;
		std::unique_ptr<TimerClass> m_Timer = nullptr;

		std::unique_ptr<ActorManagerClass> m_ActorManager = nullptr;
		std::unique_ptr<ProcessManagerClass> m_ProcessManager = nullptr;

	public:
		SystemClass(const SystemClass& other) = delete;
		SystemClass(SystemClass&& other) = delete;
		SystemClass& operator=(const SystemClass& other) = delete;
		SystemClass& operator=(SystemClass&& other) = delete;
	};

	// 윈도우 종료 또는 닫히는 메세지 처리
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

	// 외부 포인터
	static SystemClass* ApplicationHandle = nullptr;
}