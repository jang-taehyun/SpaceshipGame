#pragma once

#ifdef _DEBUG

// ImGui ฐüทร //
#pragma comment(lib, "ImGui.lib")

#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

namespace Graphic { class LightClass; }
namespace Scene { class SceneManagerClass; }
namespace Sound { class SoundManagerClass; }
namespace Object
{
	class GameObjectClass;
	class IObjectClass;
}

namespace Graphic
{
	class IMGUIClass
	{
	public:
		IMGUIClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeivceContext);
		~IMGUIClass();

		void Render(UINT FPS, UINT cpu_usage, Scene::SceneManagerClass* SceneManager, Graphic::LightClass* light);

	private:
		void Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeivceContext);
		void Shutdown();

		void SetUI(UINT FPS, UINT cpu_usage, Scene::SceneManagerClass* SceneManager, Graphic::LightClass* light);

		void SetFPSCPUUsage(const std::string& title, int IMGUI_Window_idx, int fps, int cpu_usage);
		void SetCameraInfo(const std::string& title, int IMGUI_Window_idx, Object::IObjectClass* camera);
		void SetSoundInfo(const std::string& title, int IMGUI_Window_idx, Sound::SoundManagerClass* sound_manager);
		void SetLightInfo(const std::string& title, int IMGUI_Window_idx, LightClass* light);
		void SetObjectInfo(const std::string& title, int IMGUI_Window_idx, Object::GameObjectClass* object);

	private:
		static bool IsInitialize;

		ImVec2 m_WindowsSize;
		std::vector<ImVec2> m_WindowsPositions;
		UINT m_WindowsCount = 0;

	public:
		IMGUIClass(const IMGUIClass& other) = delete;
		IMGUIClass(IMGUIClass&& other) = delete;
		IMGUIClass& operator=(const IMGUIClass& other) = delete;
		IMGUIClass& operator=(IMGUIClass&& other) = delete;
	};
}

#endif // DEBUG
