#pragma once

// IMGUI ฐüทร //
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"

class CameraClass;

class IMGUIClass
{
public:
	IMGUIClass();
	IMGUIClass(const IMGUIClass& other);
	~IMGUIClass();

	HRESULT Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext);
	void Shutdown();
	HRESULT Render(const CameraClass* const& camera);

private:
	HRESULT SetUI(const CameraClass* const& camera);
	void SetFPSCPUUsage();
	void SetCameraInfo(const CameraClass* const& camera);

private:
	ImVec2 m_WindowsSize;
	ImVec2 m_WindowsPosition[3];
	int m_WindowsCount = 0;
};

