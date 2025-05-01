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

	title = u8"카메라 위치, 회전, 카메라 이동 속도";
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
	static float origin = camera->GetKeyboardSensitivity();

	bool IsPress = false;
	float sensitive = 0.f;
	std::string contents;

	// 카메라 위치, 회전 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	ImGui::Text(u8"카메라 위치(x, y, z)");
	contents = u8"x : ";
	contents += std::to_string(camera->GetTransformObject()->GetPosition().x);
	contents += ", ";
	contents += u8"y : ";
	contents += std::to_string(camera->GetTransformObject()->GetPosition().y);
	contents += ", ";
	contents += u8"z : ";
	contents += std::to_string(camera->GetTransformObject()->GetPosition().z);
	ImGui::Text(contents.c_str());


	ImGui::Text(u8"카메라 회전(x, y, z) : ");
	contents = u8"x : ";
	contents += std::to_string(camera->GetTransformObject()->GetRotation().x);
	contents += ", ";
	contents += u8"y : ";
	contents += std::to_string(camera->GetTransformObject()->GetRotation().y);
	contents += ", ";
	contents += u8"z : ";
	contents += std::to_string(camera->GetTransformObject()->GetRotation().z);
	ImGui::Text(contents.c_str());


	// 카메라 이동 speed UI //
	sensitive = camera->GetKeyboardSensitivity();
	if (ImGui::SliderFloat(u8"카메라 속도", &sensitive, 0.0f, 0.1f))
		camera->SetKeyboardSensitivity(sensitive);

	// 초기화 //
	IsPress = ImGui::Button("reset");
	if (IsPress)
	{
		camera->SetKeyboardSensitivity(origin);
	}

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
	float value;
	DirectX::XMFLOAT4 value1;
	DirectX::XMFLOAT3 value2;

	// light 관련 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	// ambient
	value1 = light->GetAmbientColor();
	if (ImGui::SliderFloat(u8"ambient R", &value1.x, 0.0f, 1.f))
		light->SetAmbientColor(value1);
	value1 = light->GetAmbientColor();
	if (ImGui::SliderFloat(u8"ambient G", &value1.y, 0.0f, 1.f))
		light->SetAmbientColor(value1);
	value1 = light->GetAmbientColor();
	if (ImGui::SliderFloat(u8"ambient B", &value1.z, 0.0f, 1.f))
		light->SetAmbientColor(value1);
	value1 = light->GetAmbientColor();
	if (ImGui::SliderFloat(u8"ambient A", &value1.w, 0.0f, 1.f))
		light->SetAmbientColor(value1);

	// diffuse
	value1 = light->GetDiffuseColor();
	if (ImGui::SliderFloat(u8"diffuse R", &value1.x, 0.0f, 1.f))
		light->SetDiffuseColor(value1);
	value1 = light->GetDiffuseColor();
	if (ImGui::SliderFloat(u8"diffuse G", &value1.y, 0.0f, 1.f))
		light->SetDiffuseColor(value1);
	value1 = light->GetDiffuseColor();
	if (ImGui::SliderFloat(u8"diffuse B", &value1.z, 0.0f, 1.f))
		light->SetDiffuseColor(value1);
	value1 = light->GetDiffuseColor();
	if (ImGui::SliderFloat(u8"diffuse A", &value1.w, 0.0f, 1.f))
		light->SetDiffuseColor(value1);

	// direction
	value2 = light->GetDirection();
	if (ImGui::SliderFloat(u8"direction X", &value2.x, 0.0f, 1.f))
		light->SetDirection(value2);
	value2 = light->GetDirection();
	if (ImGui::SliderFloat(u8"direction Y", &value2.y, 0.0f, 1.f))
		light->SetDirection(value2);
	value2 = light->GetDirection();
	if (ImGui::SliderFloat(u8"direction Z", &value2.z, 0.0f, 1.f))
		light->SetDirection(value2);

	// specular color
	value1 = light->GetSpecularColor();
	if (ImGui::SliderFloat(u8"specular color R", &value1.x, 0.0f, 1.f))
		light->SetSpecularColor(value1);
	value1 = light->GetSpecularColor();
	if (ImGui::SliderFloat(u8"specular color G", &value1.y, 0.0f, 1.f))
		light->SetSpecularColor(value1);
	value1 = light->GetSpecularColor();
	if (ImGui::SliderFloat(u8"specular color B", &value1.z, 0.0f, 1.f))
		light->SetSpecularColor(value1);
	value1 = light->GetSpecularColor();
	if (ImGui::SliderFloat(u8"specular color A", &value1.w, 0.0f, 1.f))
		light->SetSpecularColor(value1);

	// specular power
	value = light->GetSpecularPower();
	if (ImGui::SliderFloat(u8"specular power", &value, 0.0f, 64.f))
		light->SetSpecularPower(value);

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
	static DirectX::XMFLOAT4 origin_position = actor->GetAffineObject()->GetPosition();
	static DirectX::XMFLOAT4 origin_rotate = actor->GetAffineObject()->GetRotation();
	static DirectX::XMFLOAT4 origin_scale = actor->GetAffineObject()->GetScaling();
	static DirectX::XMFLOAT3 origin_center = actor->GetCollision()->GetCollision()->Center;
	static DirectX::XMFLOAT4 origin_orientation = actor->GetCollision()->GetCollision()->Orientation;
	static DirectX::XMFLOAT3 origin_extends = actor->GetCollision()->GetCollision()->Extents;

	DirectX::XMFLOAT4 value1;
	DirectX::XMFLOAT3 value2;
	bool IsPress = false;

	// actor affine 관련 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	// position
	value1 = actor->GetAffineObject()->GetPosition();
	if (ImGui::SliderFloat(u8"position X", &value1.x, -100.0f, 100.f))
		actor->GetAffineObject()->SetPosition(value1);
	value1 = actor->GetAffineObject()->GetPosition();
	if (ImGui::SliderFloat(u8"position Y", &value1.y, -100.0f, 100.f))
		actor->GetAffineObject()->SetPosition(value1);
	value1 = actor->GetAffineObject()->GetPosition();
	if (ImGui::SliderFloat(u8"position Z", &value1.z, -100.0f, 100.f))
		actor->GetAffineObject()->SetPosition(value1);

	// rotate
	value1 = actor->GetAffineObject()->GetRotation();
	if (ImGui::SliderFloat(u8"rotate X", &value1.x, 0.f, 1.f))
		actor->GetAffineObject()->SetRotation(value1);
	value1 = actor->GetAffineObject()->GetRotation();
	if (ImGui::SliderFloat(u8"rotate Y", &value1.y, 0.f, 1.f))
		actor->GetAffineObject()->SetRotation(value1);
	value1 = actor->GetAffineObject()->GetRotation();
	if (ImGui::SliderFloat(u8"rotate Z", &value1.z, 0.f, 1.f))
		actor->GetAffineObject()->SetRotation(value1);

	// scale
	value1 = actor->GetAffineObject()->GetScaling();
	if (ImGui::SliderFloat(u8"scale X", &value1.x, -100.0f, 100.f))
		actor->GetAffineObject()->SetScale(value1);
	value1 = actor->GetAffineObject()->GetScaling();
	if (ImGui::SliderFloat(u8"scale Y", &value1.y, -100.0f, 100.f))
		actor->GetAffineObject()->SetScale(value1);
	value1 = actor->GetAffineObject()->GetScaling();
	if (ImGui::SliderFloat(u8"scale Z", &value1.z, -100.0f, 100.f))
		actor->GetAffineObject()->SetScale(value1);

	// collision position
	value2 = actor->GetCollision()->GetCollision()->Center;
	if (ImGui::SliderFloat(u8"collision center X", &value2.x, -100.0f, 100.f))
		actor->GetCollision()->SetCenter(value2);
	value2 = actor->GetCollision()->GetCollision()->Center;
	if (ImGui::SliderFloat(u8"collision center Y", &value2.y, -100.0f, 100.f))
		actor->GetCollision()->SetCenter(value2);
	value2 = actor->GetCollision()->GetCollision()->Center;
	if (ImGui::SliderFloat(u8"collision center Z", &value2.z, -100.0f, 100.f))
		actor->GetCollision()->SetCenter(value2);

	// collision rotate
	value1 = actor->GetCollision()->GetCollision()->Orientation;
	if (ImGui::SliderFloat(u8"collision rotate X", &value1.x, 0.f, 1.f))
		actor->GetCollision()->SetRotate(value1);
	value1 = actor->GetCollision()->GetCollision()->Orientation;
	if (ImGui::SliderFloat(u8"collision rotate Y", &value1.y, 0.f, 1.f))
		actor->GetCollision()->SetRotate(value1);
	value1 = actor->GetCollision()->GetCollision()->Orientation;
	if (ImGui::SliderFloat(u8"collision rotate Z", &value1.z, 0.f, 1.f))
		actor->GetCollision()->SetRotate(value1);

	// collision scale
	value2 = actor->GetCollision()->GetCollision()->Extents;
	if (ImGui::SliderFloat(u8"collision extends X", &value2.x, -100.0f, 100.f))
		actor->GetCollision()->SetExtents(value2);
	value2 = actor->GetCollision()->GetCollision()->Extents;
	if (ImGui::SliderFloat(u8"collision extends Y", &value2.y, -100.0f, 100.f))
		actor->GetCollision()->SetExtents(value2);
	value2 = actor->GetCollision()->GetCollision()->Extents;
	if (ImGui::SliderFloat(u8"collision extends Z", &value2.z, -100.0f, 100.f))
		actor->GetCollision()->SetExtents(value2);

	IsPress = ImGui::Button("reset");
	if (IsPress)
	{
		actor->GetAffineObject()->SetPosition(origin_position);
		actor->GetAffineObject()->SetRotation(origin_rotate);
		actor->GetAffineObject()->SetScale(origin_scale);
		actor->GetCollision()->SetCenter(origin_center);
		actor->GetCollision()->SetRotate(origin_orientation);
		actor->GetCollision()->SetExtents(origin_extends);
	}

	ImGui::End();
}