#pragma once

// IMGUI ฐüทร //
#pragma comment(lib, "IMGUI_library.lib")
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

class Object::IObjectClass;
class Sound::SoundClass;
class LightClass;
class Object::ActorManagerClass;

namespace Graphic
{
	const static int MaxIMGUIWindowsCount = 10;

	class IMGUIClass
	{
	public:
		IMGUIClass(HWND hwnd, const ID3D11Device* Device, const ID3D11DeviceContext* DeivceContext);
		virtual ~IMGUIClass();

		void Render(Object::ActorManagerClass* actor_manager, LightClass* light, Sound::SoundClass* sound, Object::IObjectClass* camera, int fps, int cpu_usage);

	private:
		void Initialize(HWND hwnd, const ID3D11Device* Device, const ID3D11DeviceContext* DeivceContext);
		void Shutdown();

		void SetUI(Object::ActorManagerClass* actor_manager, LightClass* light, Sound::SoundClass* sound, Object::IObjectClass* camera, int fps, int cpu_usage);

		void SetFPSCPUUsage(const std::string& title, int IMGUI_Window_idx, int fps, int cpu_usage);
		void SetCameraInfo(const std::string& title, int IMGUI_Window_idx, Object::IObjectClass* camera);
		void SetSoundInfo(const std::string& title, int IMGUI_Window_idx, Sound::SoundClass* sound);
		void SetLightInfo(const std::string& title, int IMGUI_Window_idx, LightClass* light);
		void SetActorInfo(const std::string& title, int IMGUI_Window_idx, Object::IObjectClass* actor);

	private:
		static bool IsInitialize;

		ImVec2 m_WindowsSize;
		ImVec2 m_WindowsPosition[MaxIMGUIWindowsCount];
		int m_WindowsCount = 0;

	public:
		IMGUIClass(const IMGUIClass& other) = delete;
		IMGUIClass(IMGUIClass&& other) = delete;
		IMGUIClass& operator=(const IMGUIClass& other) = delete;
		IMGUIClass& operator=(IMGUIClass&& other) = delete;
	};
}
