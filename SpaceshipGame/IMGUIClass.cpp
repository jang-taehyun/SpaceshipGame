#include "pch.h"

// FPS, CPU, Timer 관련 //
#include "SystemClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "CameraClass.h"

#include "IMGUIClass.h"

IMGUIClass::IMGUIClass() {}
IMGUIClass::IMGUIClass(const IMGUIClass& other) {}
IMGUIClass::~IMGUIClass() {}

HRESULT IMGUIClass::Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext)
{
	ImVec2 cur;
	float adder = 30.f;
	m_WindowsCount = 3;

	// IMGUI 초기화 //
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Device, DeivceContext);

	// Font 및 테마 설정 //
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
	ImGui::StyleColorsDark();

	// IMGUI의 윈도우 크기, 위치 설정 //
	m_WindowsSize = ImVec2(350.f, 150.f);

	cur = ImGui::GetMainViewport()->GetCenter();
	cur.x = 10.f;
	cur.y = 10.f;
	for (int i = 0; i < m_WindowsCount; ++i)
	{
		m_WindowsPosition[i] = ImVec2(cur.x, cur.y);
		cur.y += (m_WindowsSize.y + 10.f);
	}

	return S_OK;
}

void IMGUIClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT IMGUIClass::Render(const CameraClass* const& camera)
{
	// IMGUI 렌더링 준비 //
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetUI(camera);

	// 렌더링
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

HRESULT IMGUIClass::SetUI(const CameraClass* const& camera)
{
	SetFPSCPUUsage();
	SetCameraInfo(camera);

	return S_OK;
}

void IMGUIClass::SetFPSCPUUsage()
{
	ImVec2 pos, size;
	bool IsPress = false;
	std::string tmp;

	// FPS, CPU 사용량 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[0], ImGuiCond_Appearing);
	ImGui::Begin(u8"FPS, CPU", NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	tmp = std::to_string(SystemClass::GetSystemInst()->GetFPS()->GetFPS());
	ImGui::Text(u8"FPS : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(SystemClass::GetSystemInst()->GetCPU()->GetCPUPercentage());
	ImGui::Text(u8"CPU : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());
	IsPress = ImGui::Button("test");

	ImGui::End();
}

void IMGUIClass::SetCameraInfo(const CameraClass* const& camera)
{
	ImVec2 pos, size;
	bool IsPress = false;
	std::string tmp;

	// 카메라 위치, 회전 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[1], ImGuiCond_Appearing);
	ImGui::Begin(u8"카메라 위치, 회전값", NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	ImGui::Text(u8"카메라 위치(x, y, z)");
	tmp = std::to_string(camera->GetPosition().x);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetPosition().y);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetPosition().z);
	ImGui::Text(tmp.c_str());


	ImGui::Text(u8"카메라 회전(x, y, z) : ");
	tmp = std::to_string(camera->GetRotation().x);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetRotation().y);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetRotation().z);
	ImGui::Text(tmp.c_str());

	IsPress = ImGui::Button("test");

	ImGui::End();
}
