#pragma once

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class ModelClass;
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

class GraphicsClass
{
public:
	GraphicsClass(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	~GraphicsClass();

	HRESULT Frame(float rotationY);

private:
	HRESULT Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	void Shutdown();
	HRESULT Render();

private:
	static bool IsInitialize;

	D3DClass* m_D3D = nullptr;

	CameraClass* m_Camera = nullptr;

	ModelClass* m_Model = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	MultiTextureShaderClass* m_MultiTextureShader = nullptr;

	LightClass* m_Light = nullptr;
	LightShaderClass* m_LightShader = nullptr;

	LightMapShaderClass* m_LightMapShader = nullptr;
	AlphaMapShaderClass* m_AlphaMapShader = nullptr;
	
	TextClass* m_Text = nullptr;
	FrustumClass* m_Frustum = nullptr;
	IMGUIClass* m_IMGUI = nullptr;

public:
	GraphicsClass() = delete;
	GraphicsClass(const GraphicsClass& other) = delete;
};

bool GraphicsClass::IsInitialize = false;