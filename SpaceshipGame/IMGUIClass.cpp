#include "pch.h"

// FPS, CPU, Timer 관련 //
#include "FPSClass.h"
#include "CPUClass.h"
#include "CameraClass.h"
#include "TransformClass.h"

#include "IMGUIClass.h"

bool IMGUIClass::IsInitialize = false;
static ErrorContent e;

IMGUIClass::IMGUIClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("IMGUIClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 IMGUIClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	Initialize(hwnd, Device, DeivceContext);
	IsInitialize = true;
}

IMGUIClass::~IMGUIClass()
{
	Shutdown();
	IsInitialize = false;
}

void IMGUIClass::Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeivceContext)
{
	ImVec2 cur;
	float adder = 30.f;
	m_WindowsCount = 3;

	// 에러 메세지 초기화 //
	e.title = _T("IMGUIClass Initialize()");

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
}

void IMGUIClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void IMGUIClass::Render(CameraClass* const& camera, const int& fps, const int& cpu_usage)
{
	// IMGUI 렌더링 준비 //
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetUI(camera, fps, cpu_usage);

	// 렌더링
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void IMGUIClass::SetUI(CameraClass* const& camera, const int& fps, const int& cpu_usage)
{
	SetFPSCPUUsage(fps, cpu_usage);
	SetCameraInfo(camera);
}

void IMGUIClass::SetFPSCPUUsage(const int& fps, const int& cpu_usage)
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

	tmp = std::to_string(fps);
	ImGui::Text(u8"FPS : ");
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());

	tmp = std::to_string(cpu_usage);
	ImGui::Text(u8"CPU : ", tmp);
	ImGui::SameLine(ImGui::GetTextLineHeight(), 110.f);
	ImGui::Text(tmp.c_str());
	IsPress = ImGui::Button("test");


	ImGui::End();
}

void IMGUIClass::SetCameraInfo(CameraClass* const& camera)
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
	tmp = std::to_string(camera->GetTransformObject()->GetPosition().x);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetTransformObject()->GetPosition().y);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetTransformObject()->GetPosition().z);
	ImGui::Text(tmp.c_str());


	ImGui::Text(u8"카메라 회전(x, y, z) : ");
	tmp = std::to_string(camera->GetTransformObject()->GetRotation().x);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetTransformObject()->GetRotation().y);
	ImGui::Text(tmp.c_str());
	tmp = std::to_string(camera->GetTransformObject()->GetRotation().z);
	ImGui::Text(tmp.c_str());

	IsPress = ImGui::Button("test");

	ImGui::End();


	// 카메라 이동 speed UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[2], ImGuiCond_Appearing);
	ImGui::Begin(u8"카메라 이동 speed 조정", NULL);
	float sensitive = camera->GetKeyboardSensitivity();
	if (ImGui::SliderFloat("float", &sensitive, 0.0f, 0.1f))
		camera->SetKeyboardSensitivity(sensitive);

	IsPress = ImGui::Button("test");

	ImGui::End();
}
