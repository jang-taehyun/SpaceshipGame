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
#include "ModelListClass.h"
#include "FrustumClass.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass() {}
GraphicsClass::GraphicsClass(const GraphicsClass& other) {}
GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	// Direct3D 객체 생성 및 초기화 //
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
		return false;

	if (!m_Direct3D->Initialize(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, _T("Could not initialize Direct3D"), _T("Error"), MB_OK);
		return false;
	}


	// Camera 객체 생성 및 초기화 //
	m_Camera = new CameraClass;
	if (!m_Camera)
		return false;

	m_Camera->SetPosition(0.f, 0.f, -6.f);
	m_Camera->Render();
	DirectX::XMMATRIX BaseViewMatrix;
	m_Camera->GetViewMatrix(BaseViewMatrix);


	// Model 객체 생성 및 초기화(TextureShaderClass를 사용하는 경우) //
	m_Model = new ModelClass;
	if (!m_Model)
		return false;
	
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), _T("../Engine/data/stone01.dds"), _T("../Engine/data/dirt01.dds"), _T("../Engine/data/alpha01.dds"), "../Engine/data/cube.txt"))
	{
		MessageBox(hwnd, _T("Could not initialize the model object"), _T("Erorr"), MB_OK);
		return false;
	}

	// alpha map shader 객체 생성 및 초기화 //
	m_AlphaMapShader = new AlphaMapShaderClass;
	if (!m_AlphaMapShader)
	{
		return false;
	}

	if (!m_AlphaMapShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the alpha map shader object"), _T("Erorr"), MB_OK);
		return false;
	}

	// texture shader 객체 생성 및 초기화 //
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the texture shader object"), _T("Erorr"), MB_OK);
		return false;
	}

	// multitexture shader 객체 생성 및 초기화 //
	m_MultiTextureShader = new MultiTextureShaderClass;
	if (!m_MultiTextureShader)
	{
		return false;
	}

	
	
	if (!m_MultiTextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the multitexture shader object"), _T("Erorr"), MB_OK);
		return false;
	}

	// light shader 객체, light 객체 생성 및 초기화(LightShaderClass를 사용하는 경우) //
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the light shader object"), _T("Error"), MB_OK);
		return false;
	}

	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.f);
	m_Light->SetDiffuseColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetDirection(0.f, 0.f, 1.f);
	m_Light->SetSpecularColor(1.f, 1.f, 1.f, 1.f);
	m_Light->SetSpecularPower(64.f);

	// Light map 객체 생성 및 초기화 //
	m_LightMapShader = new LightMapShaderClass;
	if (!m_LightMapShader)
	{
		return false;
	}

	if (!m_LightMapShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, _T("Could not initialize the light map shader object"), _T("Error"), MB_OK);
		return false;
	}

	// // Text 객체 생성 및 초기화 //
	// m_Text = new TextClass;
	// if (!m_Text)
	// {
	// 	return false;
	// }
	// 
	// if (!m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, ScreenWidth, ScreenHeight, BaseViewMatrix))
	// {
	// 	MessageBox(hwnd, _T("Could not initialize the text object"), _T("Error"), MB_OK);
	// 	return false;
	// }

	// model list 객체 생성 및 초기화 //
	m_ModelList = new ModelListClass;
	if (!m_ModelList)
	{
		return false;
	}

	if (!m_ModelList->Initialize(25))
	{
		MessageBox(hwnd, _T("Could not initialize the model list object"), _T("Error"), MB_OK);
		return false;
	}

	// frustum 객체 생성 //
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = nullptr;
	}

	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = nullptr;
	}

	// if (m_Text)
	// {
	// 	m_Text->Shutdown();
	// 	delete m_Text;
	// 	m_Text = nullptr;
	// }

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

	// texture shader 객체 해제(TextureShaderClass 또는 BitmapClass를 사용하는 경우) //
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

	// color shader 객체 해제(ColorShaderClass를 사용하는 경우) //
	// if (m_ColorShader)
	// {
	// 	m_ColorShader->Shutdown();
	// 	delete m_ColorShader;
	// 	m_ColorShader = nullptr;
	// }

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

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = nullptr;
	}
}

bool GraphicsClass::Frame(int MouseX, int MouseY, int FPS, int CPU, float rotationY)
{
	// camera의 rotation을 update //
	m_Camera->SetRotation(0.f, rotationY, 0.f);

	// // mouse의 위치 update //
	// if (!m_Text->SetMousePosition(MouseX, MouseY, m_Direct3D->GetDeviceContext()))
	// {
	// 	return false;
	// }
	// 
	// // FPS update //
	// if (!m_Text->SetFPS(FPS, m_Direct3D->GetDeviceContext()))
	// {
	// 	return false;
	// }
	// 
	// // CPU 사용량 update //
	// if (!m_Text->SetCPU(CPU, m_Direct3D->GetDeviceContext()))
	// {
	// 	return false;
	// }

	// 렌더링
	return Render();
}

bool GraphicsClass::Render()
{
	// front buffer 초기화 //
	m_Direct3D->BeginScene(0.f, 0.f, 0.f, 1.f);

	// 카메라의 위치에 따라 view matrix 생성 //
	m_Camera->Render();

	// world, view, projection matrix 가져오기 //
	DirectX::XMMATRIX WorldMatrix, ViewMatrix, ProjectionMatrix, OrthoMatrix;
	m_Direct3D->GetWorldMatrix(WorldMatrix);
	m_Camera->GetViewMatrix(ViewMatrix);
	m_Direct3D->GetProjectionMatrix(ProjectionMatrix);
	m_Direct3D->GetOrthoMatrix(OrthoMatrix);

	// frustum culling을 이용한 rendering //
	// viewing frustum 생성 및 render count(rendering한 3D object의 개수) 초기화
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, ProjectionMatrix, ViewMatrix);
	int ModelCount = m_ModelList->GetModelCount();
	int RenderCount = 0;

	// 모든 3D object를 검사하면서 viewing frustum 안에 있는 3D object만 렌더링
	float pos_x, pos_y, pos_z;
	float radius = 1.f;
	DirectX::XMFLOAT4 Color;
	bool IsRender;
	for (int i = 0; i < ModelCount; i++)
	{
		// 3D object의 정보 가져오기
		m_ModelList->GetData(i, pos_x, pos_y, pos_z, Color);

		// viewing frustum 안에 3D object가 존재하는 지 확인
		IsRender = m_Frustum->CheckSphere(pos_x, pos_y, pos_z, radius);

		// 렌더링
		if (IsRender)
		{
			// 3D object를 렌더링할 위치로 이동
			WorldMatrix = DirectX::XMMatrixTranslation(pos_x, pos_y, pos_z);

			// 렌더링
			m_Model->Render(m_Direct3D->GetDeviceContext());
			// if (!m_MultiTextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTextures()))
			// {
			// 	return false;
			// }
			if (!m_AlphaMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), WorldMatrix, ViewMatrix, ProjectionMatrix, m_Model->GetTextures()))
			{
				return false;
			}

			// world matrix를 원래대로 초기화
			m_Direct3D->GetWorldMatrix(WorldMatrix);

			// 렌더링한 3D object의 개수 증가
			RenderCount++;
		}
	}

	// 2D 렌더링 //
	// // depth buffer 비활성화
	// m_Direct3D->TurnDepthBufferOff();
	// 
	// // alpha blend state 활성화
	// m_Direct3D->TurnOnAlphaBlending();
	// 
	// 
	// // texture shader로 bitmap을 렌더링
	// if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), WorldMatrix, ViewMatrix, OrthoMatrix, m_Bitmap->GetTexture()))
	// {
	// 	return false;
	// }
	// 
	// // 렌더링한 3D object의 개수 설정
	// if (!m_Text->SetRenderCount(RenderCount, m_Direct3D->GetDeviceContext()))
	// {
	// 	return false;
	// }
	// 
	// // text 렌더링
	// if (!m_Text->Render(m_Direct3D->GetDeviceContext(), WorldMatrix, OrthoMatrix))
	// {
	// 	return false;
	// }
	// 
	// // alpha blend state 비활성화
	// m_Direct3D->TurnOffAlphaBlending();
	// 
	// // depth buffer 활성화
	// m_Direct3D->TurnDepthBufferOn();
	// 
	// // back buffer에 있는 내용을 화면에 출력 //
	// m_Direct3D->EndScene();

	return true;
}
