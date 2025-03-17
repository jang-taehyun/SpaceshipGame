#include "pch.h"

// FPS, CPU, Timer ���� //
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

	// IMGUI �ʱ�ȭ //
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Device, DeivceContext);

	// Font �� �׸� ���� //
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesKorean());
	ImGui::StyleColorsDark();

	// IMGUI�� ������ ũ��, ��ġ ���� //
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
	// IMGUI ������ �غ� //
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetUI();

	// ������
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

HRESULT IMGUIClass::SetUI()
{
	ImVec2 pos, size;
	bool IsPress = false;
	std::string tmp;
	
	// 1�� â //
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


	// 2�� â //
	ImGui::SetNextWindowPos(m_WindowsPosition[1], ImGuiCond_Appearing);
	ImGui::Begin(u8"�׽�Ʈ2");
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	tmp = std::to_string(pos.x);
	ImGui::Text(u8"���� X ��ǥ : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(pos.y);
	ImGui::Text(u8"���� Y ��ǥ : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.x);
	ImGui::Text(u8"���� Width ũ�� : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.y);
	ImGui::Text(u8"���� Height ũ�� : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	IsPress = ImGui::Button("test2");
	ImGui::End();


	// 3�� â //
	ImGui::SetNextWindowPos(m_WindowsPosition[2], ImGuiCond_Appearing);
	ImGui::Begin(u8"�׽�Ʈ3");
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	tmp = std::to_string(pos.x);
	ImGui::Text(u8"���� X ��ǥ : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());
	
	tmp = std::to_string(pos.y);
	ImGui::Text(u8"���� Y ��ǥ : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.x);
	ImGui::Text(u8"���� Width ũ�� : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(size.y);
	ImGui::Text(u8"���� Height ũ�� : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	IsPress = ImGui::Button("test3");
	ImGui::End();

	

	return S_OK;
}
