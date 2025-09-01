#pragma once

namespace Graphic { class GraphicsClass; }
namespace Scene { class SceneManagerClass; }
namespace System
{
	class InputClass;
	class FPSClass;
	class CPUClass;
	class TimerClass;
}

namespace System
{
	class SystemClass
	{
	public:
		SystemClass();
		virtual ~SystemClass();

		void Run();

	private:
		void Initialize();

		void Frame();
		void InitializeWindows(UINT& ScreenWidth, UINT& ScreenHeight);
		void ShutdownWindows();

	private:
		static bool IsInitialize;

		LPCWSTR m_applicationName = _T("");

		std::unique_ptr<System::FPSClass> m_FPS = nullptr;
		std::unique_ptr<System::CPUClass> m_CPU = nullptr;
		std::unique_ptr<System::TimerClass> m_Timer = nullptr;

		std::unique_ptr<System::InputClass> m_Input = nullptr;
		std::unique_ptr<Graphic::GraphicsClass> m_Graphics = nullptr;
		std::unique_ptr<Scene::SceneManagerClass> m_SceneManager = nullptr;

	public:
		SystemClass(const SystemClass& other) = delete;
		SystemClass(SystemClass&& other) = delete;
		SystemClass& operator=(const SystemClass& other) = delete;
		SystemClass& operator=(SystemClass&& other) = delete;
	};

	// 윈도우 종료 또는 닫히는 메세지 처리
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
}