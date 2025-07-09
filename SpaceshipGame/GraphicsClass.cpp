#include "pch.h"

#include "D3DClass.h"
#include "LightClass.h"
#include "ModelManagerClass.h"
#include "ShaderManagerClass.h"
#include "UITextureManagerClass.h"
#include "UIRenderClass.h"

#include "SceneManagerClass.h"
#include "ObjectManagerClass.h"
#include "UIManagerClass.h"
#include "TextManagerClass.h"
#include "SceneClass.h"
#include "CameraClass.h"

#include "IModelClass.h"
#include "IShaderClass.h"

#include "GameObjectClass.h"
#include "IUIClass.h"
#include "ITextClass.h"
#include "TypeConverterClass.h"

#ifdef _DEBUG
#include "IMGUIClass.h"
#include "CollisionClass.h"
#endif // DEBUG

#include "GraphicsClass.h"

bool Graphic::GraphicsClass::IsInitialize = false;

Graphic::GraphicsClass::GraphicsClass(HWND hwnd, int ScreenWidth, int ScreenHeight)
{
	assert(!IsInitialize);
	Initialize(hwnd, ScreenWidth, ScreenHeight);
	IsInitialize = true;
}

Graphic::GraphicsClass::~GraphicsClass()
{
	IsInitialize = false;
}

void Graphic::GraphicsClass::Frame(Scene::SceneManagerClass* SceneManager, bool IsLoad)
{
	bool IsRender = false;
	UINT cnt = 0;
	DirectX::XMFLOAT4X4 view = {};
	Model::InstanceBufferType instance = {};
	Shader::BuffersData BufferData = {};

	if (IsLoad)
	{
		Load(SceneManager);
		return;
	}

	// frustum culling //
	// 카메라 업데이트
	static_cast<Object::CameraClass*>(SceneManager->GetCamera())->UpdateFrustum(m_D3D->GetProjectionMatrix());

	// scene에 존재하는 object에 대해 frustum culling 진행
	cnt = SceneManager->GetObjectManager()->GetObjectCount();
	for (UINT i = 0; i < cnt; ++i)
	{
		IsRender = static_cast<Object::CameraClass*>(SceneManager->GetCamera())->IsRender(
			m_ModelManager->GetModel(
				static_cast<Object::GameObjectClass*>(
					SceneManager->GetObjectManager()->GetGameObject(i)
					)->GetModelID())->GetModelOBB(),
			SceneManager->GetObjectManager()->GetGameObject(i)->GetAffineMatrix()
		);

		if (IsRender)
		{
			instance.world = SceneManager->GetObjectManager()->GetGameObject(i)->GetAffineMatrix();
			m_ModelManager->GetModel(
				static_cast<Object::GameObjectClass*>(
					SceneManager->GetObjectManager()->GetGameObject(i)
					)->GetModelID())->AddWorldMatrix(instance);
		}

#ifdef _DEBUG
		// Collision //
		IsRender = static_cast<Object::CameraClass*>(SceneManager->GetCamera())->IsRender(
			m_ModelManager->GetModel(
				static_cast<Object::CollisionClass*>(
					static_cast<Object::GameObjectClass*>(
						SceneManager->GetObjectManager()->GetGameObject(i)
						)->GetCollision()
					)->GetModelID()
			)->GetModelOBB(),
			static_cast<Object::GameObjectClass*>(
				SceneManager->GetObjectManager()->GetGameObject(i)
				)->GetCollision()->GetAffineMatrix()
		);

		if (IsRender)
		{
			instance.world = static_cast<Object::GameObjectClass*>(
				SceneManager->GetObjectManager()->GetGameObject(i)
				)->GetCollision()->GetAffineMatrix();
			instance.color = static_cast<Object::CollisionClass*>(
				static_cast<Object::GameObjectClass*>(
					SceneManager->GetObjectManager()->GetGameObject(i)
					)->GetCollision()
				)->GetColor();
			m_ModelManager->GetModel(
				static_cast<Object::CollisionClass*>(
					static_cast<Object::GameObjectClass*>(
						SceneManager->GetObjectManager()->GetGameObject(i)
						)->GetCollision()
					)->GetModelID()
			)->
				AddWorldMatrix(instance);

			instance.color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		}
			
#endif // DEBUG
	}

	// model들의 instance buffer 업데이트 //
	m_ModelManager->UpdateInstanceBuffers(m_D3D->GetDeviceContext());

	// shader의 모든 buffer 업데이트 //
	view = static_cast<Object::CameraClass*>(SceneManager->GetCamera())->Render();
	BufferData.transform.Projection = DirectX::XMLoadFloat4x4(&(m_D3D->GetProjectionMatrix()));
	BufferData.transform.View = DirectX::XMLoadFloat4x4(&view);

	BufferData.light.AmbientColor = m_Light->GetAmbientColor();
	BufferData.light.DiffuseColor = m_Light->GetDiffuseColor();
	BufferData.light.LightDirection = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(m_Light->GetDirection());
	BufferData.light.SpecularColor = m_Light->GetSpecularColor();
	BufferData.light.SpecularPower = m_Light->GetSpecularPower();

	BufferData.camera.CameraPosition = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(SceneManager->GetCamera()->GetPosition());

	m_ShaderManager->UpdateBuffer(m_D3D->GetDeviceContext(), BufferData);

	// 렌더링 //
	Render(SceneManager);
}

void Graphic::GraphicsClass::Initialize(HWND hwnd, int ScreenWidth, int ScreenHeight)
{
	DirectX::XMFLOAT4 AmbientColor = DirectX::XMFLOAT4(0.15f, 0.15f, 0.15f, 1.f);
	DirectX::XMFLOAT4 DiffuseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 LightDirection = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	DirectX::XMFLOAT4 SpecularColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float SpecularPower = 64.f;

	// Direct3D 객체 생성 및 초기화 //
	m_D3D = std::make_unique<D3DClass>(hwnd, ScreenWidth, ScreenHeight);
	assert(m_D3D);

	// Light 객체 생성 //
	m_Light = std::make_unique<LightClass>(AmbientColor, DiffuseColor, LightDirection, SpecularColor, SpecularPower);
	assert(m_Light);

	// Model manager 객체 생성 //
	m_ModelManager = std::make_unique<Model::ModelManagerClass>();
	assert(m_ModelManager);

	// Shader manager 객체 생성 //
	m_ShaderManager = std::make_unique<Shader::ShaderManagerClass>();
	assert(m_ShaderManager);

	// UI texture manager 객체 생성 //
	m_UITextureManager = std::make_unique<Texture::UITextureManagerClass>();
	assert(m_UITextureManager);

	// UI render 객체 생성 //
	m_UIRender = std::make_unique<Texture::UIRenderClass>(m_D3D->GetDeviceContext());
	assert(m_UIRender);

#ifdef _DEBUG
	// IMGUI 객체 생성 //
	m_IMGUI = std::make_unique<IMGUIClass>(GetActiveWindow(), m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	assert(m_IMGUI);
#endif // DEBUG
}

void Graphic::GraphicsClass::Load(Scene::SceneManagerClass* SceneManager)
{
	m_ModelManager->Load(GetActiveWindow(), m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetObjectManager()->GetModelMask());
	m_UITextureManager->Load(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetUIManager()->GetUITextureMask());
	m_UIRender->LoadFont(m_D3D->GetDevice(), SceneManager->GetTextManager()->GetFontMask());
	m_ShaderManager->Load(GetActiveWindow(), m_D3D->GetDevice(), m_ModelManager->GetNeedShaderMask());
}

void Graphic::GraphicsClass::Render(Scene::SceneManagerClass* SceneManager)
{
	// front buffer 초기화 //
	m_D3D->BeginScene(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	// 3D 물체 렌더링 //
	for (UINT i = 0; i < Model::ModelIDCount; ++i)
	{
		// 해당 model이 있다면 렌더링
		if (m_ModelManager->GetModel(static_cast<Model::ID>(i)))
		{
			// shader, input layout 세팅
			m_ShaderManager->GetShader(m_ModelManager->GetModel(static_cast<Model::ID>(i))->GetShaderID())->BeginRender(m_D3D->GetDeviceContext());

			for (UINT j = 0; j < m_ModelManager->GetModel(static_cast<Model::ID>(i))->GetMeshCount(); ++j)
			{
				m_ModelManager->GetModel(static_cast<Model::ID>(i))->RenderMesh(m_D3D->GetDeviceContext(), j);
				m_ShaderManager->GetShader(m_ModelManager->GetModel(static_cast<Model::ID>(i))->GetShaderID())->Render(
					m_D3D->GetDeviceContext(),
					m_ModelManager->GetModel(static_cast<Model::ID>(i))->GetIndexCount(j),
					m_ModelManager->GetModel(static_cast<Model::ID>(i))->GetInstanceCount(),
					m_ModelManager->GetModel(static_cast<Model::ID>(i))->GetMaterial(j)
				);
			}
		}
	}

	// 2D 렌더링 //
	m_UIRender->BeginRender(m_D3D.get());

	// background 렌더링
	if (SceneManager->GetUIManager()->GetBackground())
		m_UIRender->RenderBackground(
			m_UITextureManager->GetTexture(SceneManager->GetUIManager()->GetBackground()->GetUITextureID()),
			SceneManager->GetUIManager()->GetBackground()->GetColor()
		);

	// 2D UI 렌더링
	for (UINT i = 0; i < SceneManager->GetUIManager()->GetUICount(); ++i)
	{
		if (UI::State::DISAPPEAR != SceneManager->GetUIManager()->GetUI(i)->GetUIState())
			m_UIRender->RenderTexture(
				m_UITextureManager->GetTexture(SceneManager->GetUIManager()->GetUI(i)->GetUITextureID()),
				SceneManager->GetUIManager()->GetUI(i)->GetPosition(),
				SceneManager->GetUIManager()->GetUI(i)->GetColor()
			);
	}

	// text 렌더링
	for (UINT i = 0; i < SceneManager->GetTextManager()->GetTextCount(); ++i)
	{
		m_UIRender->RenderText(
			SceneManager->GetTextManager()->GetTextObject(i)->GetText(),
			SceneManager->GetTextManager()->GetTextObject(i)->GetFontID(),
			SceneManager->GetTextManager()->GetTextObject(i)->GetTextPosition(),
			SceneManager->GetTextManager()->GetTextObject(i)->GetTextColor(),
			0.f,
			DirectX::XMFLOAT2(0.f, 0.f),
			SceneManager->GetTextManager()->GetTextObject(i)->GetTextSize()
		);
	}

	m_UIRender->EndRender(m_D3D.get());

	// back buffer에 있는 내용을 화면에 출력 //
	m_D3D->EndScene();
}

#ifdef _DEBUG
void Graphic::GraphicsClass::ImGuiRender(UINT FPS, UINT cpu_usage, Scene::SceneManagerClass* SceneManager)
{
	m_IMGUI->Render(FPS, cpu_usage, SceneManager, m_Light.get());
}
#endif // DEBUG


