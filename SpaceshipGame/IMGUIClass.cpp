#include "pch.h"

// FPS, CPU, Timer, Camera, Sound, Light 관련 //
#include "FPSClass.h"
#include "CPUClass.h"
#include "CameraClass.h"
#include "SoundClass.h"
#include "LightClass.h"

// actor, collision 관련
#include "ActorManagerClass.h"
#include "ActorClass.h"
#include "PlayerClass.h"
#include "CollisionClass.h"

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
	m_WindowsCount = MaxIMGUIWindowsCount;

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
	m_WindowsSize = ImVec2(300.f, 100.f);

	cur = ImGui::GetMainViewport()->GetCenter();
	cur.x = 10.f;
	cur.y = 10.f;
	for (int i = 0; i < m_WindowsCount; ++i)
	{
		m_WindowsPosition[i] = ImVec2(cur.x, cur.y);
		cur.y += (m_WindowsSize.y + 5.f);
	}
}

void IMGUIClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void IMGUIClass::Render(ActorManagerClass* const& actor_manager, LightClass* const& light, SoundClass* const& sound, CameraClass* const& camera, const int& fps, const int& cpu_usage)
{
	// IMGUI 렌더링 준비 //
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	SetUI(actor_manager, light, sound, camera, fps, cpu_usage);

	// 렌더링
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void IMGUIClass::SetUI(ActorManagerClass* const& actor_manager, LightClass* const& light, SoundClass* const& sound, CameraClass* const& camera, const int& fps, const int& cpu_usage)
{
	std::string title = u8"FPS, CPU 사용량";
	SetFPSCPUUsage(title, 0, fps, cpu_usage);

	title = u8"카메라 위치, 회전";
	SetCameraInfo(title, 1, camera);

	title = u8"사운드 재생 조정";
	SetSoundInfo(title, 2, sound);

	title = u8"광원 정보(ambient, diffuse, direction, specular color, specular power)";
	SetLightInfo(title, 3, light);

	title = u8"player의 affine(position, rotate, scale), collision(center, rotate, extends)";
	SetActorInfo(title, 4, static_cast<ActorClass*>(actor_manager->GetPlayerObject()));
	
	for (int i = 0; i < actor_manager->GetOtherObjectCount(); ++i)
	{
		std::string title = u8"번째 other의 affine(position, rotate, scale), collision(center, rotate, extends)";
		title = std::to_string(i+1) + title;
		SetActorInfo(title, i+5, actor_manager->GetOtherObject(i));
	}
}

void IMGUIClass::SetFPSCPUUsage(const std::string& title, const int& IMGUI_Window_idx, const int& fps, const int& cpu_usage)
{
	std::string contents;

	// FPS, CPU 사용량 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	contents = u8"FPS : ";
	contents += std::to_string(fps);
	ImGui::Text(contents.c_str());

	contents = u8"CPU : ";
	contents += std::to_string(cpu_usage);
	ImGui::Text(contents.c_str());

	ImGui::End();
}

void IMGUIClass::SetCameraInfo(const std::string& title, const int& IMGUI_Window_idx, CameraClass* const& camera)
{
	bool IsPress = false;
	std::string contents;
	DirectX::XMFLOAT4 value;

	// 카메라 위치, 회전 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	ImGui::Text(u8"카메라 위치(x, y, z)");
	value = camera->GetAffineObject()->GetPosition();
	contents = u8"x : ";
	contents += std::to_string(value.x);
	contents += ", ";
	contents += u8"y : ";
	contents += std::to_string(value.y);
	contents += ", ";
	contents += u8"z : ";
	contents += std::to_string(value.z);
	ImGui::Text(contents.c_str());


	ImGui::Text(u8"카메라 회전(x, y, z)");
	value = camera->GetAffineObject()->GetRotation();
	contents = u8"x : ";
	contents += std::to_string(value.x);
	contents += ", ";
	contents += u8"y : ";
	contents += std::to_string(value.y);
	contents += ", ";
	contents += u8"z : ";
	contents += std::to_string(value.z);
	ImGui::Text(contents.c_str());

	ImGui::End();
}

void IMGUIClass::SetSoundInfo(const std::string& title, const int& IMGUI_Window_idx, SoundClass* const& sound)
{
	bool IsPress = false;

	// 사운드 재생 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);

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

void IMGUIClass::SetLightInfo(const std::string& title, const int& IMGUI_Window_idx, LightClass* const& light)
{
	static DirectX::XMFLOAT4 origin_ambient = light->GetAmbientColor();
	static DirectX::XMFLOAT4 origin_diffuse = light->GetDiffuseColor();
	static DirectX::XMFLOAT3 origin_direction = light->GetDirection();
	static DirectX::XMFLOAT4 origin_specular_color = light->GetSpecularColor();
	static float origin_specular_power = light->GetSpecularPower();

	bool IsPress = false;
	float value[4] = { 0.f, };

	// light 관련 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	// ambient
	ImGui::Text(u8"ambient color");
	ImGui::SameLine();
	value[0] = light->GetAmbientColor().x;
	value[1] = light->GetAmbientColor().y;
	value[2] = light->GetAmbientColor().z;
	value[3] = light->GetAmbientColor().w;
	if (ImGui::SliderFloat4(u8"##1", value, 0.0f, 1.f))
		light->SetAmbientColor(value[0], value[1], value[2], value[3]);

	// diffuse
	ImGui::Text(u8"diffuse color");
	ImGui::SameLine();
	value[0] = light->GetDiffuseColor().x;
	value[1] = light->GetDiffuseColor().y;
	value[2] = light->GetDiffuseColor().z;
	value[3] = light->GetDiffuseColor().w;
	if (ImGui::SliderFloat4(u8"##2", value, 0.0f, 1.f))
		light->SetDiffuseColor(value[0], value[1], value[2], value[3]);

	// direction
	ImGui::Text(u8"light direction");
	ImGui::SameLine();
	value[0] = light->GetDirection().x;
	value[1] = light->GetDirection().y;
	value[2] = light->GetDirection().z;
	if (ImGui::SliderFloat3(u8"##3", value, 0.0f, 1.f))
		light->SetDirection(value[0], value[1], value[2]);

	// specular color
	ImGui::Text(u8"specular color(alpha, red, green, blue)");
	ImGui::SameLine();
	value[0] = light->GetSpecularColor().x;
	value[1] = light->GetSpecularColor().y;
	value[2] = light->GetSpecularColor().z;
	value[3] = light->GetSpecularColor().w;
	if (ImGui::SliderFloat4(u8"##4", value, 0.0f, 1.f))
		light->SetSpecularColor(value[0], value[1], value[2], value[3]);

	// specular power
	ImGui::Text(u8"specular power");
	ImGui::SameLine();
	value[0] = light->GetSpecularPower();
	if (ImGui::SliderFloat(u8"##5", &value[0], 0.0f, 64.f))
		light->SetSpecularPower(value[0]);

	IsPress = ImGui::Button("reset");
	if (IsPress)
	{
		light->SetAmbientColor(origin_ambient);
		light->SetDiffuseColor(origin_diffuse);
		light->SetDirection(origin_direction);
		light->SetSpecularColor(origin_specular_color);
		light->SetSpecularPower(origin_specular_power);
	}

	ImGui::End();
}

void IMGUIClass::SetActorInfo(const std::string& title, const int& IMGUI_Window_idx, ActorClass* const& actor)
{
	static float keyboard_origin = actor->GetAffineObject()->GetKeyboardSensitivity();
	static float mouse_origin = actor->GetAffineObject()->GetMouseSensitivity();
	static DirectX::XMFLOAT4 origin_position = actor->GetAffineObject()->GetPosition();
	static DirectX::XMFLOAT4 origin_rotate = actor->GetAffineObject()->GetRotation();
	static DirectX::XMFLOAT4 origin_scale = actor->GetAffineObject()->GetScaling();
	static DirectX::XMFLOAT3 origin_center = actor->GetCollision()->GetCollision()->Center;
	static DirectX::XMFLOAT4 origin_orientation = actor->GetCollision()->GetCollision()->Orientation;
	static DirectX::XMFLOAT3 origin_extends = actor->GetCollision()->GetCollision()->Extents;

	float value[3] = { 0.f, };
	bool IsPress = false;
	float sensitive;

	// actor affine 관련 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	// position
	value[0] = actor->GetAffineObject()->GetPosition().x;
	value[1] = actor->GetAffineObject()->GetPosition().y;
	value[2] = actor->GetAffineObject()->GetPosition().z;
	ImGui::Text(u8"위치");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##1", value, -100.0f, 100.f))
		actor->GetAffineObject()->SetPosition(value[0], value[1], value[2]);

	// rotate
	value[0] = actor->GetAffineObject()->GetRotation().x;
	value[1] = actor->GetAffineObject()->GetRotation().y;
	value[2] = actor->GetAffineObject()->GetRotation().z;
	ImGui::Text(u8"회전");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##2", value, -100.0f, 100.f))
		actor->GetAffineObject()->SetRotation(value[0], value[1], value[2]);

	// scale
	value[0] = actor->GetAffineObject()->GetScaling().x;
	value[1] = actor->GetAffineObject()->GetScaling().y;
	value[2] = actor->GetAffineObject()->GetScaling().z;
	ImGui::Text(u8"크기");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##3", value, -100.0f, 100.f))
		actor->GetAffineObject()->SetScale(value[0], value[1], value[2]);

	// collision position
	value[0] = actor->GetCollision()->GetCollision()->Center.x;
	value[1] = actor->GetCollision()->GetCollision()->Center.y;
	value[2] = actor->GetCollision()->GetCollision()->Center.z;
	ImGui::Text(u8"충돌체 위치");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##4", value, -100.0f, 100.f))
		actor->GetCollision()->SetCenter(value[0], value[1], value[2]);

	// collision rotate
	value[0] = actor->GetCollision()->GetCollision()->Orientation.x;
	value[1] = actor->GetCollision()->GetCollision()->Orientation.y;
	value[2] = actor->GetCollision()->GetCollision()->Orientation.z;
	ImGui::Text(u8"충돌체 회전");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##5", value, -100.0f, 100.f))
		actor->GetCollision()->SetRotate(value[0], value[1], value[2]);

	// collision scale
	value[0] = actor->GetCollision()->GetCollision()->Extents.x;
	value[1] = actor->GetCollision()->GetCollision()->Extents.y;
	value[2] = actor->GetCollision()->GetCollision()->Extents.z;
	ImGui::Text(u8"충돌체 크기");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##6", value, -100.0f, 100.f))
		actor->GetCollision()->SetExtents(value[0], value[1], value[2]);

	// 이동 speed UI //
	ImGui::Text(u8"이동 속도");
	ImGui::SameLine();
	sensitive = actor->GetAffineObject()->GetKeyboardSensitivity();
	if (ImGui::SliderFloat(u8"##7", &sensitive, 0.0f, 0.1f))
		actor->GetAffineObject()->SetKeyboardSensitivity(sensitive);

	// 회전 speed UI //
	ImGui::Text(u8"회전 속도");
	ImGui::SameLine();
	sensitive = actor->GetAffineObject()->GetMouseSensitivity();
	if (ImGui::SliderFloat(u8"##8", &sensitive, 0.0f, 0.002f, "%.10f"))
		actor->GetAffineObject()->SetMouseSensitivity(sensitive);

	IsPress = ImGui::Button("reset");
	if (IsPress)
	{
		actor->GetAffineObject()->SetKeyboardSensitivity(keyboard_origin);
		actor->GetAffineObject()->SetMouseSensitivity(mouse_origin);
		actor->GetAffineObject()->SetPosition(origin_position);
		actor->GetAffineObject()->SetRotation(origin_rotate);
		actor->GetAffineObject()->SetScale(origin_scale);
		actor->GetCollision()->SetCenter(origin_center);
		actor->GetCollision()->SetRotate(origin_orientation);
		actor->GetCollision()->SetExtents(origin_extends);
	}

	ImGui::End();
}