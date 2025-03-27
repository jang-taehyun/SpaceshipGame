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
#include "IMGUIClass.h"

#include "InputClass.h"

#include "GraphicsClass.h"

bool GraphicsClass::IsInitialize = false;

GraphicsClass::GraphicsClass(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd)
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 GraphicsClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;

		throw e;
	}

	result = Initialize(ScreenWidth, ScreenHeight, hwnd);
	if (FAILED(result))
	{
		Shutdown();

		e.contents = _T("GraphicsClass 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	IsInitialize = true;
}

GraphicsClass::~GraphicsClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT GraphicsClass::Initialize(const int& ScreenWidth, const int& ScreenHeight, const HWND& hwnd)
{
	ErrorContent e;
	HRESULT result = S_OK;
	DirectX::XMMATRIX BaseViewMatrix;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Initialize()");

	// Direct3D 객체 생성 및 초기화 //
	m_D3D = new D3DClass(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!m_D3D)
	{
		e.contents = _T("D3DClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	// Camera 객체 생성 및 초기화 //
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		e.contents = _T("CameraClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		throw e;
	}
	m_Camera->Render();
	m_Camera->GetViewMatrix(BaseViewMatrix);

	// Model 객체 생성 및 초기화 //
	m_Model = new ModelClass;
	if (!m_Model)
		return E_FAIL;
	if (FAILED(m_Model->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), CubeTextureFileNames, CubeModelFileName)))
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
	if (FAILED(m_AlphaMapShader->Initialize(m_D3D->GetDevice(), hwnd, AlphaMapShaderInfo)))
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
	if (FAILED(m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd, TextureShaderInfo)))
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
	if (FAILED(m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd, MultiTextureShaderInfo)))
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
	if (FAILED(m_LightShader->Initialize(m_D3D->GetDevice(), hwnd, LightShaderInfo)))
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
	if (FAILED(m_LightMapShader->Initialize(m_D3D->GetDevice(), hwnd, LightMapShaderInfo)))
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
	if (FAILED(m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, ScreenWidth, ScreenHeight, BaseViewMatrix)))
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

	// IMGUI 세팅 //
	m_IMGUI = new IMGUIClass(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_IMGUI)
	{
		e.contents = _T("IMGUIClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		throw e;
	}

	return result;
}

void GraphicsClass::Shutdown()
{
	if (m_IMGUI)
	{
		delete m_IMGUI;
		m_IMGUI = nullptr;
	}

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

HRESULT GraphicsClass::Frame(const InputClass* const& input, const float& frame, const int& fps, const int& cpu_usage)
{
	ErrorContent e;
	HRESULT result = S_OK;
	bool KeyDown = false;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Frame()");

	// camera 객체의 frame time 갱신 및 keyboard, mouse의 input에 따라 camera 객체의 transform 변경 //
	m_Camera->GetTransformObject()->SetFrameTime(frame);

	KeyDown = input->IsLeftArrowPressed();
	result = m_Camera->GetTransformObject()->ChangeRotation(RotationState::ROTATE_LEFT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("camera 객체의 transform 변경(LEFT) 실패");
		e.errorCode = result;
		throw e;
	}

	KeyDown = input->IsRightArrowPressed();
	result = m_Camera->GetTransformObject()->ChangeRotation(RotationState::ROTATE_RIGHT, KeyDown);
	if (FAILED(result))
	{
		e.contents = _T("camera 객체의 transform 변경(RIGHT) 진행 실패");
		e.errorCode = result;
		throw e;
	}

	// 렌더링 //
	result = Render(fps, cpu_usage);
	if (FAILED(result))
	{
		e.contents = _T("Frame() 처리 실패");
		e.errorCode = result;
		throw e;
	}

	return result;
}

HRESULT GraphicsClass::Render(const int& fps, const int& cpu_usage)
{
	ErrorContent e;
	HRESULT result = S_OK;
	DirectX::XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix, OrthoMatrix;
	ColorClass background;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Render()");

	// front buffer 초기화 //
	m_D3D->BeginScene(background);

	// 카메라의 위치에 따라 view matrix 생성 //
	m_Camera->Render();

	// world, view, projection, ortho matrix 가져오기 및 업데이트 //
	// world matrix
	m_D3D->GetWorldMatrix(WorldMatrix);
	WorldMatrix = m_Model->GetAffineMatrix();

	// view matrix
	m_Camera->GetViewMatrix(ViewMatrix);

	// projection matrix
	m_D3D->GetProjectionMatrix(ProjectionMatrix);

	// ortho matrix
	m_D3D->GetOrthoMatrix(OrthoMatrix);

	// frustum culling을 이용한 rendering //
	// viewing frustum 생성 및 render count(rendering한 3D object의 개수) 초기화
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, ProjectionMatrix, ViewMatrix);

	// 렌더링 //
	m_Model->Render(m_D3D->GetDeviceContext());
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTextureArray());
	if (FAILED(result))
	{
		e.contents = _T("Model 렌더링 실패");
		e.errorCode = result;
		throw e;
	}

	// 2D 렌더링 //
	// depth buffer 비활성화
	m_D3D->TurnDepthBufferOff();
	
	// alpha blend state 활성화
	m_D3D->TurnOnAlphaBlending();
	
	// text 렌더링
	result = m_Text->Render(m_D3D->GetDeviceContext(), WorldMatrix, OrthoMatrix);
	if (FAILED(result))
	{
		e.contents = _T("text 렌더링 실패");
		e.errorCode = result;
		throw e;
	}
	
	// alpha blend state 비활성화
	m_D3D->TurnOffAlphaBlending();
	
	// depth buffer 활성화
	m_D3D->TurnDepthBufferOn();
	
	// IMGUI 렌더링
	m_IMGUI->Render(m_Camera, fps, cpu_usage);

	// back buffer에 있는 내용을 화면에 출력 //
	m_D3D->EndScene();

	return result;
}
