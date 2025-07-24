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
#include "InputClass.h"
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

void Graphic::GraphicsClass::Frame(HWND hwnd, Scene::SceneManagerClass* SceneManager, bool IsLoad
#ifdef _DEBUG
	, System::InputClass* input
#endif // _DEBUG
)
{
	bool IsRender = false;
	UINT cnt = 0;
	DirectX::XMFLOAT4X4 view = {};
	Model::InstanceBufferType instance = {};
	Shader::BuffersData BufferData = {};
	Object::GameObjectClass* obj = nullptr;
	Object::CameraClass* cam = static_cast<Object::CameraClass*>(SceneManager->GetCamera());

#ifdef _DEBUG
	Object::CollisionClass* col = nullptr;
#endif

	if (IsLoad)
	{
		Load(hwnd, SceneManager);
		return;
	}

	// frustum culling //
	// 카메라 업데이트
	assert(cam);
	view = cam->Render();
	cam->UpdateFrustum(m_D3D->GetProjectionMatrix());

	// scene에 존재하는 object에 대해 frustum culling 진행
	cnt = SceneManager->GetObjectManager()->GetObjectCount();
	for (UINT i = 0; i < cnt; ++i)
	{
		obj = static_cast<Object::GameObjectClass*>(SceneManager->GetObjectManager()->GetGameObject(i));
		assert(obj);

		IsRender = cam->IsRender(
			m_ModelManager->GetModel(obj->GetModelID())->GetModelOBB(),
			obj->GetAffineMatrix()
		);

		if (IsRender)
		{
			instance.world = obj->GetAffineMatrix();
			m_ModelManager->GetModel(obj->GetModelID())->AddWorldMatrix(instance);
		}

#ifdef _DEBUG
		// Collision
		col = static_cast<Object::CollisionClass*>(obj->GetCollision());
		assert(col);

		IsRender = cam->IsRender(
			m_ModelManager->GetModel(obj->GetModelID())->GetModelOBB(),
			col->GetAffineMatrix()
		);

		if (IsRender)
		{
			instance.world = col->GetAffineMatrix();
			instance.color = col->GetColor();
			m_ModelManager->GetModel(col->GetModelID())->AddWorldMatrix(instance);

			instance.color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		}
			
#endif // DEBUG
	}

	// model들의 instance buffer 업데이트 //
	m_ModelManager->UpdateInstanceBuffers(m_D3D->GetDeviceContext());

	// shader의 모든 buffer 업데이트 //
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
	Render(hwnd, SceneManager
#ifdef _DEBUG
		, input
#endif // _DEBUG
	);
}

void Graphic::GraphicsClass::Initialize(HWND hwnd, int ScreenWidth, int ScreenHeight)
{
	DirectX::XMFLOAT4 AmbientColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 DiffuseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 LightDirection = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
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
	m_IMGUI = std::make_unique<IMGUIClass>(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	assert(m_IMGUI);
#endif // DEBUG
}

void Graphic::GraphicsClass::Load(HWND hwnd, Scene::SceneManagerClass* SceneManager)
{
	m_ModelManager->Load(hwnd, m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetObjectManager()->GetModelMask());
	m_UITextureManager->Load(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetUIManager()->GetUITextureMask());
	m_UIRender->LoadFont(m_D3D->GetDevice(), SceneManager->GetTextManager()->GetFontMask());
	m_ShaderManager->Load(hwnd, m_D3D->GetDevice(), m_ModelManager->GetNeedShaderMask());
}

void Graphic::GraphicsClass::Render(HWND hwnd, Scene::SceneManagerClass* SceneManager
#ifdef _DEBUG
	, System::InputClass* input
#endif // _DEBUG
)
{
	Model::IModelClass* model = nullptr;
	Shader::IShaderClass* shader = nullptr;
	UI::IUIClass* ui = nullptr;
	Text::ITextClass* text = nullptr;

	// front buffer 초기화 //
	m_D3D->BeginScene(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	// 3D 물체 렌더링 //
	for (UINT i = 0; i < Model::ModelIDCount; ++i)
	{
		model = m_ModelManager->GetModel(static_cast<Model::ID>(i));

		// 해당 model이 있다면 렌더링
		if (model)
		{
			// shader, input layout 세팅
			shader = m_ShaderManager->GetShader(model->GetShaderID());
			shader->BeginRender(m_D3D->GetDeviceContext());

			for (UINT j = 0; j < model->GetMeshCount(); ++j)
			{
				model->RenderMesh(m_D3D->GetDeviceContext(), j);
				shader->Render(
					m_D3D->GetDeviceContext(),
					model->GetIndexCount(j),
					model->GetInstanceCount(),
					model->GetMaterial(j)
				);
			}
		}
	}

	// 2D 렌더링 //
	m_UIRender->BeginRender(m_D3D.get());

	// 2D UI 렌더링
	for (UINT i = 0; i < SceneManager->GetUIManager()->GetUICount(); ++i)
	{
		ui = SceneManager->GetUIManager()->GetUI(i);
		if (UI::State::DISAPPEAR != ui->GetUIState())
		{
			// background를 제외한 모든 UI 렌더링
			if (UI::ID::BACKGROUND != ui->GetUIID())
			{
				m_UIRender->RenderTexture(
					m_UITextureManager->GetTexture(ui->GetUITextureID()),
					ui->GetPosition(),
					ui->GetColor(),
					ui->GetRotation(),
					ui->GetOrigin(),
					ui->GetScale()
				);
			}
			// background 렌더링
			else
				m_UIRender->RenderBackground(hwnd, m_UITextureManager->GetTexture(ui->GetUITextureID()), ui->GetColor());
		}
	}

	// text 렌더링
	for (UINT i = 0; i < SceneManager->GetTextManager()->GetTextCount(); ++i)
	{
		text = SceneManager->GetTextManager()->GetTextObject(i);
		m_UIRender->RenderText(
			text->GetText(),
			text->GetFontID(),
			text->GetPosition(),
			text->GetColor(),
			text->GetRotation(),
			text->GetOrigin(),
			text->GetScale()
		);
	}

	m_UIRender->EndRender(m_D3D.get());

#ifdef _DEBUG
	ImGuiRender(SceneManager, input);
#endif // DEBUG

	// back buffer에 있는 내용을 화면에 출력 //
	m_D3D->EndScene();
}

#ifdef _DEBUG
void Graphic::GraphicsClass::ImGuiRender(Scene::SceneManagerClass* SceneManager, System::InputClass* input)
{
	m_IMGUI->Render(SceneManager, m_Light.get(), input);
}
#endif // DEBUG


