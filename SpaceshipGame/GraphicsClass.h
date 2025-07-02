#pragma once

namespace Graphic
{
	class D3DClass;
	class IMGUIClass;
	class LightClass;

	namespace Model { class ModelManagerClass; }
}

class FrustumClass;

namespace Graphic
{
	class GraphicsClass
	{
	public:
		GraphicsClass(int ScreenWidth, int ScreenHeight, HWND hwnd);
		virtual ~GraphicsClass();

		void Frame(ActorManagerClass* actor_manager, SoundClass* sound, int fps, int cpu_usage, const std::wstring& scene_info);

	private:
		void Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd);
		HRESULT Render(ActorManagerClass* actor_manager, SoundClass* sound, int fps, int cpu_usage, const std::wstring& scene_info);

	private:
		static bool IsInitialize;

		D3DClass* m_D3D = nullptr;

		LightClass* m_Light = nullptr;
		ModelManagerClass* m_ModelManager = nullptr;

		TextRenderClass* m_TextRender = nullptr;
		FrustumClass* m_Frustum = nullptr;
		IMGUIClass* m_IMGUI = nullptr;

	public:
		GraphicsClass(const GraphicsClass& other) = delete;
		GraphicsClass(GraphicsClass&& other) = delete;
		GraphicsClass& operator=(const GraphicsClass& other) = delete;
		GraphicsClass& operator=(GraphicsClass&& other) = delete;
	};
}