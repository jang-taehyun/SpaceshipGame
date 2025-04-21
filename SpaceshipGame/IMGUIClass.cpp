#include "pch.h"

// FPS, CPU, Timer, Camera, Sound, Light 관련 //
#include "FPSClass.h"
#include "CPUClass.h"
#include "CameraClass.h"
#include "SoundClass.h"
#include "LightClass.h"

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
	m_WindowsCount = 6;

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

void IMGUIClass::Render(LightClass* const& light, ModelClass* const& model, SoundClass* const& sound, CameraClass* const& camera, const int& fps, const int& cpu_usage)
{
	// IMGUI 렌더링 준비 //
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetUI(light, model, sound, camera, fps, cpu_usage);

	// 렌더링
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void IMGUIClass::SetUI(LightClass* const& light, ModelClass* const& model, SoundClass* const& sound, CameraClass* const& camera, const int& fps, const int& cpu_usage)
{
	SetFPSCPUUsage(fps, cpu_usage);
	SetCameraInfo(camera);
	SetSoundInfo(sound);
	SetLightInfo(light);
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
	float sensitive = 0.f;
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


	// 카메라 이동 speed UI //
	sensitive = camera->GetKeyboardSensitivity();
	if (ImGui::SliderFloat(u8"카메라 속도", &sensitive, 0.0f, 0.1f))
		camera->SetKeyboardSensitivity(sensitive);

	IsPress = ImGui::Button("test");

	ImGui::End();
}

void IMGUIClass::SetSoundInfo(SoundClass* const& sound)
{
	bool IsPress = false;

	// 사운드 재생 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[2], ImGuiCond_Appearing);
	ImGui::Begin(u8"사운드 재생 조정", NULL);

	// backgound 오디오 //
	IsPress = ImGui::Button(u8"백그라운드");
	if (IsPress)
	{
		if (sound->IsBackgoundPlay())
			sound->StopWaveFile(SoundInfo::BACKGROUND);
		else
			sound->PlayWaveFile(SoundInfo::BACKGROUND);
	}

	// effect 오디오 //
	IsPress = ImGui::Button(u8"효과음");
	if (IsPress)
	{
		if (sound->IsEffectPlay())
			sound->StopWaveFile(SoundInfo::EFFECT);
		else
			sound->PlayWaveFile(SoundInfo::EFFECT);
	}

	ImGui::End();
}

void IMGUIClass::SetLightInfo(LightClass* const& light)
{
	ImVec2 pos, size;
	bool IsPress = false;
	float value = 0.f;
	std::string tmp;

	// light 관련 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[3], ImGuiCond_Appearing);
	ImGui::Begin(u8"광원 정보(ambient, diffuse, direction, specular color, specular power", NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	pos = ImGui::GetWindowPos();
	size = ImGui::GetWindowSize();

	// ambient
	value = light->GetAmbientColor().x;
	if (ImGui::SliderFloat(u8"ambient R", &value, 0.0f, 1.f))
		light->SetAmbientColor(value, light->GetAmbientColor().y, light->GetAmbientColor().z, light->GetAmbientColor().w);
	value = light->GetAmbientColor().y;
	if (ImGui::SliderFloat(u8"ambient G", &value, 0.0f, 1.f))
		light->SetAmbientColor(light->GetAmbientColor().x, value, light->GetAmbientColor().z, light->GetAmbientColor().w);
	value = light->GetAmbientColor().z;
	if (ImGui::SliderFloat(u8"ambient B", &value, 0.0f, 1.f))
		light->SetAmbientColor(light->GetAmbientColor().x, light->GetAmbientColor().y, value, light->GetAmbientColor().w);
	value = light->GetAmbientColor().w;
	if (ImGui::SliderFloat(u8"ambient A", &value, 0.0f, 1.f))
		light->SetAmbientColor(light->GetAmbientColor().x, light->GetAmbientColor().y, light->GetAmbientColor().z, value);

	// diffuse
	value = light->GetDiffuseColor().x;
	if (ImGui::SliderFloat(u8"diffuse R", &value, 0.0f, 1.f))
		light->SetDiffuseColor(value, light->GetDiffuseColor().y, light->GetDiffuseColor().z, light->GetDiffuseColor().w);
	value = light->GetDiffuseColor().y;
	if (ImGui::SliderFloat(u8"diffuse G", &value, 0.0f, 1.f))
		light->SetDiffuseColor(light->GetDiffuseColor().x, value, light->GetDiffuseColor().z, light->GetDiffuseColor().w);
	value = light->GetDiffuseColor().z;
	if (ImGui::SliderFloat(u8"diffuse B", &value, 0.0f, 1.f))
		light->SetDiffuseColor(light->GetDiffuseColor().x, light->GetDiffuseColor().y, value, light->GetDiffuseColor().w);
	value = light->GetDiffuseColor().w;
	if (ImGui::SliderFloat(u8"diffuse A", &value, 0.0f, 1.f))
		light->SetDiffuseColor(light->GetDiffuseColor().x, light->GetDiffuseColor().y, light->GetDiffuseColor().z, value);

	// direction
	value = light->GetDirection().x;
	if (ImGui::SliderFloat(u8"direction X", &value, 0.0f, 1.f))
		light->SetDirection(value, light->GetDirection().y, light->GetDirection().z);
	value = light->GetDirection().y;
	if (ImGui::SliderFloat(u8"direction Y", &value, 0.0f, 1.f))
		light->SetDirection(light->GetDirection().x, value, light->GetDirection().z);
	value = light->GetDirection().z;
	if (ImGui::SliderFloat(u8"direction Z", &value, 0.0f, 1.f))
		light->SetDirection(light->GetDirection().x, light->GetDirection().y, value);

	// specular color
	value = light->GetSpecularColor().x;
	if (ImGui::SliderFloat(u8"specular color R", &value, 0.0f, 1.f))
		light->SetSpecularColor(value, light->GetSpecularColor().y, light->GetSpecularColor().z, light->GetSpecularColor().w);
	value = light->GetSpecularColor().y;
	if (ImGui::SliderFloat(u8"specular color G", &value, 0.0f, 1.f))
		light->SetSpecularColor(light->GetSpecularColor().x, value, light->GetSpecularColor().z, light->GetSpecularColor().w);
	value = light->GetSpecularColor().z;
	if (ImGui::SliderFloat(u8"specular color B", &value, 0.0f, 1.f))
		light->SetSpecularColor(light->GetSpecularColor().x, light->GetSpecularColor().y, value, light->GetSpecularColor().w);
	value = light->GetSpecularColor().w;
	if (ImGui::SliderFloat(u8"specular color A", &value, 0.0f, 1.f))
		light->SetSpecularColor(light->GetSpecularColor().x, light->GetSpecularColor().y, light->GetSpecularColor().z, value);

	// specular power
	value = light->GetSpecularPower();
	if (ImGui::SliderFloat(u8"specular power", &value, 0.0f, 10000.f))
		light->SetSpecularPower(value);

	IsPress = ImGui::Button("test");

	ImGui::End();
}
