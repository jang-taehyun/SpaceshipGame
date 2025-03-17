#include "pch.h"

// FPS, CPU, Timer 관련 //
#include "SystemClass.h"
#include "TimerClass.h"
#include "FPSClass.h"
#include "CPUClass.h"

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

HRESULT IMGUIClass::Render()
{
	// IMGUI 렌더링 준비 //
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetUI();

	// 렌더링
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

HRESULT IMGUIClass::SetUI()
{
	ImVec2 pos, size;
	bool IsPress = false;
	std::string tmp;
	
	// 1번 창 //
	ImGui::SetNextWindowPos(m_WindowsPosition[0], ImGuiCond_Appearing);
	ImGui::Begin(u8"FPS, CPU, Timer", NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	tmp = std::to_string(SystemClass::GetSystemInst()->GetTimer()->GetTime());
	ImGui::Text(u8"Timer : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(SystemClass::GetSystemInst()->GetFPS()->GetFPS());
	ImGui::Text(u8"FPS : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(SystemClass::GetSystemInst()->GetCPU()->GetCPUPercentage());
	ImGui::Text(u8"CPU : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	ImGui::End();


	// 2번 창 //
	ImGui::SetNextWindowPos(m_WindowsPosition[1], ImGuiCond_Appearing);
	ImGui::Begin(u8"테스트2");
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	tmp = std::to_string(pos.x);
	ImGui::Text(u8"현재 X 좌표 : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(pos.y);
	ImGui::Text(u8"현재 Y 좌표 : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.x);
	ImGui::Text(u8"현재 Width 크기 : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.y);
	ImGui::Text(u8"현재 Height 크기 : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	IsPress = ImGui::Button("test2");
	ImGui::End();


	// 3번 창 //
	ImGui::SetNextWindowPos(m_WindowsPosition[2], ImGuiCond_Appearing);
	ImGui::Begin(u8"테스트3");
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	tmp = std::to_string(pos.x);
	ImGui::Text(u8"현재 X 좌표 : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());
	
	tmp = std::to_string(pos.y);
	ImGui::Text(u8"현재 Y 좌표 : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.x);
	ImGui::Text(u8"현재 Width 크기 : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.y);
	ImGui::Text(u8"현재 Height 크기 : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	IsPress = ImGui::Button("test3");
	ImGui::End();

	

	return S_OK;
}
