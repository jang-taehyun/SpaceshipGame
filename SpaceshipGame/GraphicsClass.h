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
class ModelListClass;
class FrustumClass;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, int, int, float);

private:
	bool Render();

private:
	D3DClass* m_Direct3D = nullptr;
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

