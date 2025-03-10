#include "pch.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"			// -> TextureShaderClass 또는 BitmapClass를 사용하는 경우
#include "MultiTextureShaderClass.h"
#include "LightClass.h"					// -> LightShaderClass를 사용하는 경우
#include "LightShaderClass.h"			// -> LightShaderClass를 사용하는 경우
#include "LightMapShaderClass.h"
#include "AlphaMapShaderClass.h"
#include "TextClass.h"
#include "FrustumClass.h"
#include "ColorClass.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass() {}
GraphicsClass::GraphicsClass(const GraphicsClass& other) {}
GraphicsClass::~GraphicsClass() {}

HRESULT GraphicsClass::Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd)
{
	if (!D3DClass::GetD3DClassInst(hwnd))
	{
		MessageBox(hwnd, _T("D3D 객체 생성 실패!"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// Direct3D 객체 생성 및 초기화 //
	if (FAILED(D3DClass::GetD3DClassInst(hwnd)->Initialize(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)))
	{
		MessageBox(hwnd, _T("Could not initialize Direct3D"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// Camera 객체 생성 및 초기화 //
	m_Camera = new CameraClass;
	if (!m_Camera)
		return E_FAIL;
	m_Camera->SetPosition(0.f, 0.f, -6.f);
	m_Camera->Render();
	DirectX::XMMATRIX BaseViewMatrix;
	m_Camera->GetViewMatrix(BaseViewMatrix);

	// Model 객체 생성 및 초기화 //
	m_Model = new ModelClass;
	if (!m_Model)
		return E_FAIL;
	if (FAILED(m_Model->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), D3DClass::GetD3DClassInst(hwnd)->GetDeviceContext(), CubeTextureFileNames, CubeModelFileName)))
	{
		MessageBox(hwnd, _T("Could not initialize the model object"), _T("Erorr"), MB_OK);
		return E_FAIL;
	}

	// alpha map shader 객체 생성 및 초기화 //
	m_AlphaMapShader = new AlphaMapShaderClass;
	if (!m_AlphaMapShader)
	{
		return E_FAIL;
	}
	if (FAILED(m_AlphaMapShader->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), hwnd, AlphaMapShaderInfo)))
	{
		MessageBox(hwnd, _T("Could not initialize the alpha map shader object"), _T("Erorr"), MB_OK);
		return E_FAIL;
	}

	// texture shader 객체 생성 및 초기화 //
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return E_FAIL;
	}
	if (FAILED(m_TextureShader->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), hwnd, TextureShaderInfo)))
	{
		MessageBox(hwnd, _T("Could not initialize the texture shader object"), _T("Erorr"), MB_OK);
		return E_FAIL;
	}

	// multitexture shader 객체 생성 및 초기화 //
	m_MultiTextureShader = new MultiTextureShaderClass;
	if (!m_MultiTextureShader)
	{
		return E_FAIL;
	}
	if (FAILED(m_MultiTextureShader->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), hwnd, MultiTextureShaderInfo)))
	{
		MessageBox(hwnd, _T("Could not initialize the multitexture shader object"), _T("Erorr"), MB_OK);
		return E_FAIL;
	}

	// light shader 객체 생성 및 초기화  //
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return E_FAIL;
	}
	if (FAILED(m_LightShader->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), hwnd, LightShaderInfo)))
	{
		MessageBox(hwnd, _T("Could not initialize the light shader object"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// light 객체 생성 및 초기화 //
	m_Light = new LightClass;
	if (!m_Light)
	{
		return E_FAIL;
	}
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetDirection(0.f, 0.f, 1.f);
	m_Light->SetSpecularColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetSpecularPower(64.f);

	// Light map shader 객체 생성 및 초기화 //
	m_LightMapShader = new LightMapShaderClass;
	if (!m_LightMapShader)
	{
		return E_FAIL;
	}
	if (FAILED(m_LightMapShader->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), hwnd, LightMapShaderInfo)))
	{
		MessageBox(hwnd, _T("Could not initialize the light map shader object"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// Text 객체 생성 및 초기화 //
	m_Text = new TextClass;
	if (!m_Text)
	{
		return E_FAIL;
	}
	if (FAILED(m_Text->Initialize(D3DClass::GetD3DClassInst(hwnd)->GetDevice(), D3DClass::GetD3DClassInst(hwnd)->GetDeviceContext(), hwnd, ScreenWidth, ScreenHeight, BaseViewMatrix)))
	{
		MessageBox(hwnd, _T("Could not initialize the text object"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	// frustum 객체 생성 //
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return E_FAIL;
	}

	return S_OK;
}

void GraphicsClass::Shutdown()
{
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = nullptr;
	}

	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = nullptr;
	}

	if (m_LightMapShader)
	{
		m_LightMapShader->Shutdown();
		delete m_LightMapShader;
		m_LightMapShader = nullptr;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

	// texture shader 객체 해제 //
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	// multitexture shader 객체 해제 //
	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = nullptr;
	}

	if (m_AlphaMapShader)
	{
		m_AlphaMapShader->Shutdown();
		delete m_AlphaMapShader;
		m_AlphaMapShader = nullptr;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}
}

HRESULT GraphicsClass::Frame(float rotationY, const HWND& hwnd)
{
	// camera의 rotation을 update //
	m_Camera->SetRotation(0.f, rotationY, 0.f);

	// 렌더링
	return Render(hwnd);
}

HRESULT GraphicsClass::Render(const HWND& hwnd)
{
	// front buffer 초기화 //
	ColorClass background;
	D3DClass::GetD3DClassInst(hwnd)->BeginScene(background);

	// 카메라의 위치에 따라 view matrix 생성 //
	m_Camera->Render();

	// world, view, projection matrix 가져오기 //
	DirectX::XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix, OrthoMatrix;
	D3DClass::GetD3DClassInst(hwnd)->GetWorldMatrix(WorldMatrix);
	m_Camera->GetViewMatrix(ViewMatrix);
	D3DClass::GetD3DClassInst(hwnd)->GetProjectionMatrix(ProjectionMatrix);
	D3DClass::GetD3DClassInst(hwnd)->GetOrthoMatrix(OrthoMatrix);

	// frustum culling을 이용한 rendering //
	// viewing frustum 생성 및 render count(rendering한 3D object의 개수) 초기화
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, ProjectionMatrix, ViewMatrix);

	// 렌더링
	m_Model->Render(D3DClass::GetD3DClassInst(hwnd)->GetDeviceContext());
	if (FAILED(m_MultiTextureShader->Render(D3DClass::GetD3DClassInst(hwnd)->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTextureArray())))
	{
		return E_FAIL;
	}
	if (FAILED(m_AlphaMapShader->Render(D3DClass::GetD3DClassInst(hwnd)->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTextureArray())))
	{
		return E_FAIL;
	}

	// 2D 렌더링 //
	// depth buffer 비활성화
	D3DClass::GetD3DClassInst(hwnd)->TurnDepthBufferOff();
	
	// alpha blend state 활성화
	D3DClass::GetD3DClassInst(hwnd)->TurnOnAlphaBlending();
	
	// 렌더링한 3D object의 개수 설정
	// if (FAILED(m_Text->SetRenderCount(RenderCount, m_Direct3D->GetDeviceContext())))
	// {
	// 	return E_FAIL;
	// }
	
	// text 렌더링
	if (FAILED(m_Text->Render(D3DClass::GetD3DClassInst(hwnd)->GetDeviceContext(), WorldMatrix, OrthoMatrix)))
	{
		return E_FAIL;
	}
	
	// alpha blend state 비활성화
	D3DClass::GetD3DClassInst(hwnd)->TurnOffAlphaBlending();
	
	// depth buffer 활성화
	D3DClass::GetD3DClassInst(hwnd)->TurnDepthBufferOn();
	

	// back buffer에 있는 내용을 화면에 출력 //
	D3DClass::GetD3DClassInst(hwnd)->EndScene();

	return S_OK;
}
