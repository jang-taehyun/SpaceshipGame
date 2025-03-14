#include "pch.h"

// IMGUI ���� //
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"

#include "IMGUIClass.h"

IMGUIClass::IMGUIClass() {}
IMGUIClass::IMGUIClass(const IMGUIClass& other) {}
IMGUIClass::~IMGUIClass() {}

HRESULT IMGUIClass::Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Device, DeivceContext);
	ImGui::StyleColorsDark();

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
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImVec2 cur = center;
	bool IsPress = false;
	float adder = 90.f;

	cur.x = cur.x - WIDTH / 2 + 10.f;
	cur.y = cur.y - HEIGHT / 2 + 10.f;

	// 1�� â //
	ImGui::SetNextWindowPos(ImVec2(cur.x, cur.y), ImGuiCond_Appearing);
	ImGui::Begin("�׽�Ʈ1", NULL);
	ImGui::SetWindowSize(ImVec2(50.f, 60.f), ImGuiCond_Once);
	ImGui::Text(std::to_string(cur.x).c_str());
	ImGui::Text(std::to_string(cur.y).c_str());
	IsPress = ImGui::Button("test1");
	ImGui::End();

	// 2�� â //
	ImGui::SetNextWindowPos(ImVec2(cur.x, cur.y += adder), ImGuiCond_Appearing);
	ImGui::Begin("�׽�Ʈ2");
	ImGui::SetWindowSize(ImVec2(50.f, 60.f), ImGuiCond_Once);
	ImGui::Text(std::to_string(cur.x).c_str());
	ImGui::Text(std::to_string(cur.y).c_str());
	IsPress = ImGui::Button("test2");
	ImGui::End();

	// 3�� â //
	ImGui::SetNextWindowPos(ImVec2(cur.x, cur.y += adder), ImGuiCond_Appearing);
	ImGui::Begin("�׽�Ʈ3");
	ImGui::SetWindowSize(ImVec2(50.f, 60.f), ImGuiCond_Once);
	ImGui::Text(std::to_string(cur.x).c_str());
	ImGui::Text(std::to_string(cur.y).c_str());
	IsPress = ImGui::Button("test3");
	ImGui::End();

	// ������
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

HRESULT IMGUIClass::Render(const HWND& hwnd)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImVec2 cur = center;
	bool IsPress = false;
	float adder = 90.f;

	cur.x = cur.x - WIDTH / 2 + 10.f;
	cur.y = cur.y - HEIGHT / 2 + 10.f;

	// 1�� â //
	ImGui::SetNextWindowPos(ImVec2(cur.x, cur.y), ImGuiCond_Appearing);
	ImGui::Begin("�׽�Ʈ1", NULL);
	ImGui::SetWindowSize(ImVec2(50.f, 60.f), ImGuiCond_Once);
	ImGui::Text(std::to_string(cur.x).c_str());
	ImGui::Text(std::to_string(cur.y).c_str());
	IsPress = ImGui::Button("test1");
	if (IsPress)
		MessageBox(hwnd, _T("test1 ��ư ����"), _T("test1 button"), MB_OK);
	ImGui::End();

	// 2�� â //
	ImGui::SetNextWindowPos(ImVec2(cur.x, cur.y += adder), ImGuiCond_Appearing);
	ImGui::Begin("�׽�Ʈ2");
	ImGui::SetWindowSize(ImVec2(50.f, 60.f), ImGuiCond_Once);
	ImGui::Text(std::to_string(cur.x).c_str());
	ImGui::Text(std::to_string(cur.y).c_str());
	IsPress = ImGui::Button("test2");
	if (IsPress)
		MessageBox(hwnd, _T("test2 ��ư ����"), _T("test2 button"), MB_OK);
	ImGui::End();

	// 3�� â //
	ImGui::SetNextWindowPos(ImVec2(cur.x, cur.y += adder), ImGuiCond_Appearing);
	ImGui::Begin("�׽�Ʈ3");
	ImGui::SetWindowSize(ImVec2(50.f, 60.f), ImGuiCond_Once);
	ImGui::Text(std::to_string(cur.x).c_str());
	ImGui::Text(std::to_string(cur.y).c_str());
	IsPress = ImGui::Button("test3");
	if (IsPress)
		MessageBox(hwnd, _T("test3 ��ư ����"), _T("test3 button"), MB_OK);
	ImGui::End();

	// ������
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}
