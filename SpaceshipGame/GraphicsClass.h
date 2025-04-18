#pragma once

class D3DClass;
class ModelManagerClass;
class CameraClass;
class TextClass;
class FrustumClass;
class IMGUIClass;

class InputClass;
class SoundClass;
class ActorClass;

class GraphicsClass
{
public:
	explicit GraphicsClass(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	virtual ~GraphicsClass();

	HRESULT Frame(SoundClass* const& sound, const InputClass* const& input, const float& frame, const int& fps, const int& cpu_usage);

private:
	HRESULT Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	void Shutdown();
	HRESULT Render(SoundClass* const& sound, const int& fps, const int& cpu_usage);

private:
	static bool IsInitialize;

	D3DClass* m_D3D = nullptr;

	CameraClass* m_Camera = nullptr;
	ActorClass* m_Player = nullptr;

	ModelManagerClass* m_ModelManager = nullptr;
	
	TextClass* m_Text = nullptr;
	FrustumClass* m_Frustum = nullptr;
	IMGUIClass* m_IMGUI = nullptr;

	bool m_IsShowingCursor = true;

public:
	GraphicsClass() = delete;
	GraphicsClass(const GraphicsClass& other) = delete;
};