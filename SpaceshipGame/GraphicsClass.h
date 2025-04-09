#pragma once

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
// class ModelClass;
class ModelManagerClass;
class CameraClass;
class TextureShaderClass;
class MultiTextureShaderClass;
class LightClass;
class LightShaderClass;	
class LightMapShaderClass;
class AlphaMapShaderClass;
class TextClass;
class FrustumClass;
class IMGUIClass;

class InputClass;

class GraphicsClass
{
public:
	GraphicsClass(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	~GraphicsClass();

	HRESULT Frame(const InputClass* const& input, const float& frame, const int& fps, const int& cpu_usage);

private:
	HRESULT Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	void Shutdown();
	HRESULT Render(const int& fps, const int& cpu_usage);

private:
	static bool IsInitialize;

	D3DClass* m_D3D = nullptr;

	CameraClass* m_Camera = nullptr;

	// ModelClass* m_Model = nullptr;
	ModelManagerClass* m_ModelManager = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	MultiTextureShaderClass* m_MultiTextureShader = nullptr;

	LightClass* m_Light = nullptr;
	LightShaderClass* m_LightShader = nullptr;

	LightMapShaderClass* m_LightMapShader = nullptr;
	AlphaMapShaderClass* m_AlphaMapShader = nullptr;
	
	TextClass* m_Text = nullptr;
	FrustumClass* m_Frustum = nullptr;
	IMGUIClass* m_IMGUI = nullptr;

	bool m_IsShowingCursor = true;

public:
	GraphicsClass() = delete;
	GraphicsClass(const GraphicsClass& other) = delete;
};