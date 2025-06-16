#pragma once

class D3DClass;
class ModelManagerClass;
class ActorManagerClass;
class CameraClass;
class TextRenderClass;
class FrustumClass;
class IMGUIClass;
class LightClass;

class InputClass;
class SoundClass;

class GraphicsClass
{
public:
	GraphicsClass(int ScreenWidth, int ScreenHeight, HWND hwnd);
	virtual ~GraphicsClass();

	HRESULT Frame(ActorManagerClass* actor_manager, SoundClass* sound, int fps, int cpu_usage, const std::wstring& scene_info);

	inline const CameraClass* GetCamera() const { return m_Camera; }

private:
	HRESULT Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd);
	void Shutdown();
	HRESULT Render(ActorManagerClass* actor_manager, SoundClass* sound, int fps, int cpu_usage, const std::wstring& scene_info);

private:
	static bool IsInitialize;

	D3DClass* m_D3D = nullptr;

	CameraClass* m_Camera = nullptr;
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