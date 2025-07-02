#include "pch.h"

#include "D3DClass.h"
#include "LightClass.h"

#include "ModelManagerClass.h"
#include "ActorManagerClass.h"

#include "FrustumClass.h"
#include "IMGUIClass.h"

#include "GraphicsClass.h"

bool Graphic::GraphicsClass::IsInitialize = false;

Graphic::GraphicsClass::GraphicsClass(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	assert(!IsInitialize);

	Initialize(ScreenWidth, ScreenHeight, hwnd);
	IsInitialize = true;
}

Graphic::GraphicsClass::~GraphicsClass()
{
	IsInitialize = false;
}

void Graphic::GraphicsClass::Initialize(int ScreenWidth, int ScreenHeight, HWND hwnd)
{
	HRESULT result = S_OK;
	float ScalingFactor = 0.5f;
	AffineInfo affine = {};
	DirectX::XMMATRIX BaseViewMatrix;
	DirectX::XMFLOAT4 AmbientColor = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 1.f);;
	DirectX::XMFLOAT4 DiffuseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);;
	DirectX::XMFLOAT3 LightDirection = DirectX::XMFLOAT3(0.f, 0.f, 1.f);
	DirectX::XMFLOAT4 SpecularColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float SpecularPower = 64.f;

	// Direct3D 객체 생성 및 초기화 //
	m_D3D = new D3DClass(ScreenWidth, ScreenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!m_D3D)
	{
		e.contents = _T("D3DClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// Camera 객체 생성 및 초기화 //
	affine.position = { 0.f, 3.35f, -3.35f, 1.f };
	affine.rotation = { 0.32f, 0.006f, 0.f, 1.f };
	affine.scale = { 1.f, 1.f, 1.f, 1.f };
	m_Camera = new CameraClass(affine);
	if (!m_Camera)
	{
		e.contents = _T("CameraClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// Base view matrix 가져오기 //
	BaseViewMatrix = m_Camera->GetViewMatrix();

	// Model manager 객체 생성 및 초기화 //
	m_ModelManager = new ModelManagerClass(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if(!m_ModelManager)
	{
		e.contents = _T("Model Manager Class 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// Text 객체 생성 및 초기화 //
	m_TextRender = new TextRenderClass(m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_TextRender)
	{
		e.contents = _T("TextClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// frustum 객체 생성 //
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		e.contents = _T("FrustumClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// IMGUI 객체 생성 //
	m_IMGUI = new IMGUIClass(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	if (!m_IMGUI)
	{
		e.contents = _T("IMGUIClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// Light 객체 생성 //
	m_Light = new LightClass(AmbientColor, DiffuseColor, LightDirection, SpecularColor, SpecularPower);
	if (!m_Light)
	{
		e.contents = _T("lightClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}
}

void Graphic::GraphicsClass::Frame(ActorManagerClass* const& actor_manager, SoundClass* const& sound, const int& fps, const int& cpu_usage, const std::wstring& scene_info)
{
	HRESULT result = S_OK;

	// 렌더링 //
	result = Render(actor_manager, sound, fps, cpu_usage, scene_info);
	if (FAILED(result))
	{
		e.contents = _T("Frame() 처리 실패");
		e.errorCode = result;
		return result;
	}
}

HRESULT Graphic::GraphicsClass::Render(ActorManagerClass* const& actor_manager, SoundClass* const& sound, const int& fps, const int& cpu_usage, const std::wstring& scene_info)
{
	HRESULT result = S_OK;
	DirectX::XMMATRIX OrthoMatrix;
	TransformMatrixData transform = {};
	ColorClass background;

	// 에러 메세지 초기화 //
	e.title = _T("GraphicsClass Render()");

	// front buffer 초기화 //
	m_D3D->BeginScene(background);

	// 카메라의 위치에 따라 view matrix 생성 //
	DirectX::XMFLOAT4 cpos = actor_manager->GetPlayerObject()->GetAffineObject()->GetPosition();
	DirectX::XMFLOAT4 crot = actor_manager->GetPlayerObject()->GetAffineObject()->GetRotation();
	cpos.y += 2.35f;
	crot.x += 0.32f;
	crot.y += 0.006f;
	m_Camera->GetAffineObject()->SetPosition(cpos);
	m_Camera->GetAffineObject()->SetRotation(crot);
	m_Camera->Render();

	// view, projection, ortho matrix 가져오기 및 업데이트 //
	transform.view = m_Camera->GetViewMatrix();										// view matrix
	transform.projection = m_D3D->GetProjectionMatrix();							// projection matrix
	OrthoMatrix = m_D3D->GetOrthoMatrix();											// ortho matrix

	// frustum culling을 이용한 rendering //
	// viewing frustum 업데이트 및 render count(rendering한 3D object의 개수) 초기화
	m_Frustum->UpdateFrustum(SCREEN_DEPTH, transform.projection, transform.view);

	// 렌더링 //
	// player model
	transform.world = actor_manager->GetPlayerObject()->GetAffineObject()->GetAffine();
	result = m_ModelManager->GetModel(actor_manager->GetPlayerObject()->GetModelID())->Render(m_D3D->GetDeviceContext(), transform, m_Light, m_Camera);
	if (FAILED(result))
	{
		e.contents = _T("Model 렌더링 실패");
		e.errorCode = result;
		return result;
	}

	// player collision
	transform.world = actor_manager->GetPlayerObject()->GetCollision()->GetAffineObject()->GetAffine();
	static_cast<CubeModelClass*>(m_ModelManager->GetModel(ModelIDs::DEFAULT_CUBE))->SetColor(actor_manager->GetPlayerObject()->GetCollision()->GetColor());
	result = m_ModelManager->GetModel(ModelIDs::DEFAULT_CUBE)->Render(m_D3D->GetDeviceContext(), transform, m_Light, m_Camera);
	if (FAILED(result))
	{
		e.contents = _T("Collision 렌더링 실패");
		e.errorCode = result;
		return result;
	}

	// other
	for (int i = 0; i < actor_manager->GetOtherObjectCount(); ++i)
	{
		transform.world = actor_manager->GetOtherObject(i)->GetAffineObject()->GetAffine();
		result = m_ModelManager->GetModel(actor_manager->GetOtherObject(i)->GetModelID())->Render(m_D3D->GetDeviceContext(), transform, m_Light, m_Camera);
		if (FAILED(result))
		{
			e.contents = _T("Model 렌더링 실패");
			e.errorCode = result;
			return result;
		}
		transform.world = actor_manager->GetOtherObject(i)->GetCollision()->GetAffineObject()->GetAffine();
		static_cast<CubeModelClass*>(m_ModelManager->GetModel(ModelIDs::DEFAULT_CUBE))->SetColor(actor_manager->GetOtherObject(i)->GetCollision()->GetColor());
		result = m_ModelManager->GetModel(ModelIDs::DEFAULT_CUBE)->Render(m_D3D->GetDeviceContext(), transform, m_Light, m_Camera);
		if (FAILED(result))
		{
			e.contents = _T("Collision 렌더링 실패");
			e.errorCode = result;
			return result;
		}
	}

	// 2D 렌더링 //
	// text 렌더링
	DirectX::XMFLOAT2 pos = { 500.f, 600.f };
	DirectX::XMFLOAT4 color = { 1.f, 1.f, 1.f, 1.f };
	m_TextRender->Render(m_D3D, scene_info, pos, color);
	
	// IMGUI 렌더링
	m_IMGUI->Render(actor_manager, m_Light, sound, m_Camera, fps, cpu_usage);

	// back buffer에 있는 내용을 화면에 출력 //
	m_D3D->EndScene();

	return result;
}
