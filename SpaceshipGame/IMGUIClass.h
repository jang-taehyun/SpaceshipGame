#pragma once

// IMGUI ฐüทร //
#pragma comment(lib, "IMGUI.lib")

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

const static int MaxIMGUIWindowsCount = 10;

class CameraClass;
class SoundClass;
class ModelClass;
class LightClass;
class ActorManagerClass;

class IMGUIClass
{
public:
	explicit IMGUIClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext);
	virtual ~IMGUIClass();
	
	void Render(ActorManagerClass* const& actor_manager, LightClass* const& light, SoundClass* const& sound, CameraClass* const& camera, const int& fps, const int& cpu_usage);

private:
	void Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext);
	void Shutdown();

	void SetUI(ActorManagerClass* const& actor_manager, LightClass* const& light, SoundClass* const& sound, CameraClass* const& camera, const int& fps, const int& cpu_usage);
	
	void SetFPSCPUUsage(const std::string& title, const int& IMGUI_Window_idx, const int& fps, const int& cpu_usage);
	void SetCameraInfo(const std::string& title, const int& IMGUI_Window_idx, CameraClass* const& camera);
	void SetSoundInfo(const std::string& title, const int& IMGUI_Window_idx, SoundClass* const& sound);
	void SetLightInfo(const std::string& title, const int& IMGUI_Window_idx, LightClass* const& light);
	void SetActorInfo(const std::string& title, const int& IMGUI_Window_idx, ActorClass* const& actor);

private:
	static bool IsInitialize;

	ImVec2 m_WindowsSize;
	ImVec2 m_WindowsPosition[MaxIMGUIWindowsCount];
	int m_WindowsCount = 0;

public:
	IMGUIClass() = delete;
	IMGUIClass(const IMGUIClass& other) = delete;
};