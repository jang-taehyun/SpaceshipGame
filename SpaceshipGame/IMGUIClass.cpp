#include "pch.h"

// FPS, CPU, Timer, Camera, Sound, Light 관련 //
#include "FPSClass.h"
#include "CPUClass.h"
#include "CameraClass.h"
#include "SoundClass.h"
#include "LightClass.h"

// actor, collision 관련
#include "ActorManagerClass.h"
#include "IObjectClass.h"
#include "MoveableObjectClass.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "ActorClass.h"

#include "IMGUIClass.h"

bool Graphic::IMGUIClass::IsInitialize = false;

Graphic::IMGUIClass::IMGUIClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeivceContext)
{
	HRESULT result = S_OK;

	assert(IsInitialize);

	Initialize(hwnd, Device, DeivceContext);
	IsInitialize = true;
}

Graphic::IMGUIClass::~IMGUIClass()
{
	Shutdown();
	IsInitialize = false;
}

void Graphic::IMGUIClass::Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeivceContext)
{
	ImVec2 cur = {};
	m_WindowsCount = MaxIMGUIWindowsCount;

	// IMGUI 초기화 //
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(const_cast<ID3D11Device*>(Device), const_cast<ID3D11DeviceContext*>(DeivceContext));

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

void Graphic::IMGUIClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Graphic::IMGUIClass::Render(Object::ActorManagerClass* actor_manager, LightClass* light, Sound::SoundClass* sound, Object::IObjectClass* camera, int fps, int cpu_usage)
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

void Graphic::IMGUIClass::SetUI(Object::ActorManagerClass* actor_manager, LightClass* light, Sound::SoundClass* sound, Object::IObjectClass* camera, int fps, int cpu_usage)
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
	SetActorInfo(title, 4, actor_manager->GetPlayerInterface());
	
	for (int i = 0; i < actor_manager->GetActorInterfaceCount(); ++i)
	{
		if (!(*actor_manager)[i])
			continue;

		std::string title = u8"번째 other의 affine(position, rotate, scale), collision(center, rotate, extends)";
		title = std::to_string(i+1) + title;
		SetActorInfo(title, i + 5, (*actor_manager)[i]);
	}
}

void Graphic::IMGUIClass::SetFPSCPUUsage(const std::string& title, int IMGUI_Window_idx, int fps, int cpu_usage)
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

void Graphic::IMGUIClass::SetCameraInfo(const std::string& title, int IMGUI_Window_idx, Object::IObjectClass* camera)
{
	bool IsPress = false;
	std::string contents;
	DirectX::XMFLOAT4 value = {};

	// 카메라 위치, 회전 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	ImGui::Text(u8"카메라 위치(x, y, z)");
	value = camera->GetPosition();
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
	value = camera->GetRotation();
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

void Graphic::IMGUIClass::SetSoundInfo(const std::string& title, int IMGUI_Window_idx, Sound::SoundClass* sound)
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
			sound->StopWaveFile(Sound::ID::BACKGROUND);
		else
			sound->PlayWaveFile(Sound::ID::BACKGROUND);
	}

	// effect 오디오 //
	IsPress = ImGui::Button(u8"효과음");
	if (IsPress)
	{
		if (sound->IsEffectPlay())
			sound->StopWaveFile(Sound::ID::EFFECT);
		else
			sound->PlayWaveFile(Sound::ID::EFFECT);
	}

	ImGui::End();
}

void Graphic::IMGUIClass::SetLightInfo(const std::string& title, int IMGUI_Window_idx, LightClass* light)
{
	static DirectX::XMFLOAT4 origin_ambient = light->GetAmbientColor();
	static DirectX::XMFLOAT4 origin_diffuse = light->GetDiffuseColor();
	static DirectX::XMFLOAT4 origin_direction = light->GetDirection();
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

void Graphic::IMGUIClass::SetActorInfo(const std::string& title, int IMGUI_Window_idx, Object::IObjectClass* actor)
{
	float value[4] = { 0.f, };
	bool IsPress = false;
	float speed = 0.f;

	// actor affine 관련 UI //
	ImGui::SetNextWindowPos(m_WindowsPosition[IMGUI_Window_idx], ImGuiCond_Appearing);
	ImGui::Begin(title.c_str(), NULL);
	ImGui::SetWindowSize(m_WindowsSize, ImGuiCond_Once);

	// position
	value[0] = actor->GetPosition().x;
	value[1] = actor->GetPosition().y;
	value[2] = actor->GetPosition().z;
	ImGui::Text(u8"위치");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##1", value, -100.0f, 100.f))
		actor->SetPosition(value[0], value[1], value[2]);

	// rotate
	value[0] = actor->GetRotation().x;
	value[1] = actor->GetRotation().y;
	value[2] = actor->GetRotation().z;
	ImGui::Text(u8"회전");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##2", value, -100.0f, 100.f))
		actor->SetRotation(value[0], value[1], value[2]);

	// scale
	value[0] = actor->GetScale().x;
	value[1] = actor->GetScale().y;
	value[2] = actor->GetScale().z;
	ImGui::Text(u8"크기");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##3", value, -100.0f, 100.f))
		actor->SetScale(value[0], value[1], value[2]);

	// collision position
	value[0] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().x;
	value[1] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().y;
	value[2] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().z;
	ImGui::Text(u8"충돌체 위치");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##4", value, -100.0f, 100.f))
		static_cast<Object::ActorClass*>(actor)->SetPosition(value[0], value[1], value[2]);

	// collision rotate
	value[0] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().x;
	value[1] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().y;
	value[2] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().z;
	ImGui::Text(u8"충돌체 회전");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##5", value, -100.0f, 100.f))
		static_cast<Object::ActorClass*>(actor)->SetRotation(value[0], value[1], value[2]);

	// collision scale
	value[0] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().x;
	value[1] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().y;
	value[2] = static_cast<Object::ActorClass*>(actor)->GetCollision()->GetPosition().z;
	ImGui::Text(u8"충돌체 크기");
	ImGui::SameLine();
	if (ImGui::SliderFloat3(u8"##6", value, -100.0f, 100.f))
		static_cast<Object::ActorClass*>(actor)->SetScale(value[0], value[1], value[2]);

	// 이동 speed UI //
	ImGui::Text(u8"이동 속도");
	ImGui::SameLine();
	speed = static_cast<Object::MoveableObjectClass*>(actor)->GetMoveObject()->GetMoveSpeed();
	if (ImGui::SliderFloat(u8"##7", &speed, 0.0f, 0.1f))
		static_cast<Object::MoveableObjectClass*>(actor)->GetMoveObject()->SetMoveSpeed(speed);

	// 회전 speed UI //
	ImGui::Text(u8"회전 속도");
	ImGui::SameLine();
	speed = static_cast<Object::MoveableObjectClass*>(actor)->GetRotateObject()->GetRoteteSpeed();
	if (ImGui::SliderFloat(u8"##8", &speed, 0.0f, 0.002f, "%.10f"))
		static_cast<Object::MoveableObjectClass*>(actor)->GetRotateObject()->SetRoteteSpeed(speed);

	ImGui::End();
}