#pragma once

// IMGUI ฐüทร //
#pragma comment(lib, "IMGUI.lib")

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

class CameraClass;

class IMGUIClass
{
public:
	IMGUIClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext);
	~IMGUIClass();
	
	void Render(const CameraClass* const& camera, const int& fps, const int& cpu_usage);

private:
	void Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext);
	void Shutdown();

	void SetUI(const CameraClass* const& camera, const int& fps, const int& cpu_usage);
	void SetFPSCPUUsage(const int& fps, const int& cpu_usage);
	void SetCameraInfo(const CameraClass* const& camera);

private:
	static bool IsInitialize;

	ImVec2 m_WindowsSize;
	ImVec2 m_WindowsPosition[3];
	int m_WindowsCount = 0;

public:
	IMGUIClass() = delete;
	IMGUIClass(const IMGUIClass& other) = delete;
};