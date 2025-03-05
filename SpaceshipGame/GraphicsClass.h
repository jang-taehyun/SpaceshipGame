#pragma once

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class ModelClass;
class CameraClass;
class TextureShaderClass;
class MultiTextureShaderClass;
class LightClass;
class LightShaderClass;	
class LightMapShaderClass;
class AlphaMapShaderClass;
class TextClass;
class ModelListClass;
class FrustumClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass& other);
	~GraphicsClass();

	HRESULT Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd);
	void Shutdown();
	HRESULT Frame(int, int, int, int, float);

private:
	HRESULT Render();

private:
	CameraClass* m_Camera = nullptr;
	ModelClass* m_Model = nullptr;
	TextureShaderClass* m_TextureShader = nullptr;
	MultiTextureShaderClass* m_MultiTextureShader = nullptr;
	LightShaderClass* m_LightShader = nullptr;
	LightMapShaderClass* m_LightMapShader = nullptr;
	AlphaMapShaderClass* m_AlphaMapShader = nullptr;
	LightClass* m_Light = nullptr;
	TextClass* m_Text = nullptr;
	ModelListClass* m_ModelList = nullptr;
	FrustumClass* m_Frustum = nullptr;
};

