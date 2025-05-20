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
class ActorClass;

class GraphicsClass
{
public:
	explicit GraphicsClass(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	virtual ~GraphicsClass();

	HRESULT Frame(ActorManagerClass* const& actor_manager, SoundClass* const& sound, const int& fps, const int& cpu_usage, const std::wstring& scene_info);

	inline CameraClass* const& GetCamera() const { return m_Camera; }

private:
	HRESULT Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	void Shutdown();
	HRESULT Render(ActorManagerClass* const& actor_manager, SoundClass* const& sound, const int& fps, const int& cpu_usage, const std::wstring& scene_info);

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
	GraphicsClass() = delete;
	GraphicsClass(const GraphicsClass& other) = delete;
};