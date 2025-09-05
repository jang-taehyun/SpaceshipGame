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
#include "TerrainClass.h"

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

Graphic::GraphicsClass::GraphicsClass(int ScreenWidth, int ScreenHeight)
{
	assert(!IsInitialize);
	Initialize(ScreenWidth, ScreenHeight);
	IsInitialize = true;
}

Graphic::GraphicsClass::~GraphicsClass()
{
	IsInitialize = false;
}

void Graphic::GraphicsClass::Frame(Scene::SceneManagerClass* SceneManager, bool IsLoad
#ifdef _DEBUG
	, System::InputClass* input
#endif // _DEBUG
)
{
	bool IsRender = false;
	UINT cnt = 0, camIdx = 0;
	DirectX::XMFLOAT4X4 view = {};
	DirectX::XMFLOAT4X4 proj = {};
	Model::InstanceBufferType instance = {};
	Shader::BuffersData BufferData = {};
	Object::GameObjectClass* obj = nullptr;
	Object::CameraClass* cam = nullptr;
	Model::IModelClass* model = nullptr;

#ifdef _DEBUG
	Object::CollisionClass* col = nullptr;
#endif

	if (IsLoad)
	{
		Load(SceneManager);
		return;
	}

	// 카메라 가져오기
	cam = static_cast<Object::CameraClass*>(SceneManager->GetObjectManager()->GetCamera());

	// frustum culling //
	if (cam)
	{
		// 카메라 렌더링을 통해 view matrix 업데이트
		view = cam->Render();

		// view matrix, projection matrix를 이용해 frustum 업데이트
		cam->UpdateFrustum(m_D3D->GetProjectionMatrix());

		// scene에 존재하는 object에 대해 frustum culling 진행
		cnt = SceneManager->GetObjectManager()->GetObjectCount();
		camIdx = SceneManager->GetObjectManager()->GetCameraIdx();
		for (UINT i = 0; i < cnt; ++i)
		{
			if (i != camIdx)
			{
				// object 가져오기
				obj = static_cast<Object::GameObjectClass*>(SceneManager->GetObjectManager()->GetGameObject(i));
				assert(obj);

				// object의 model 가져오기
				model = m_ModelManager->GetModel(obj->GetModelID());
				assert(model);

				// frustum culling 진행
				IsRender = cam->IsRenderModel(model->GetModelOBB(), obj->GetAffineMatrix());

				// frustum 내에 있다면 object의 affine을 해당 instance buffer에 넣기
				if (IsRender)
				{
					instance.world = obj->GetAffineMatrix();
					instance.color = obj->GetColor();
					model->AddWorldMatrix(instance);
				}

#ifdef _DEBUG
				// Collision 가져오기
				col = static_cast<Object::CollisionClass*>(obj->GetCollision());

				if (col)
				{
					// collision의 model 가져오기
					model = m_ModelManager->GetModel(col->GetModelID());
					assert(model);

					// frustum culling 진행
					IsRender = cam->IsRenderModel(model->GetModelOBB(), col->GetAffineMatrix());

					// frustum 내에 있다면 object의 affine을 해당 instance buffer에 넣기
					if (IsRender)
					{
						instance.world = col->GetAffineMatrix();
						instance.color = col->GetColor();
						model->AddWorldMatrix(instance);
					}
				}
#endif // DEBUG
			}
		}

		// model들의 instance buffer 업데이트 //
		m_ModelManager->UpdateInstanceBuffers(m_D3D->GetDeviceContext());

		// shader의 모든 buffer 업데이트 //
		proj = m_D3D->GetProjectionMatrix();
		BufferData.transform.Projection = DirectX::XMLoadFloat4x4(&proj);
		BufferData.transform.View = DirectX::XMLoadFloat4x4(&view);

		BufferData.light.AmbientColor = m_Light->GetAmbientColor();
		BufferData.light.DiffuseColor = m_Light->GetDiffuseColor();
		BufferData.light.LightDirection = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(m_Light->GetDirection());
		BufferData.light.SpecularColor = m_Light->GetSpecularColor();
		BufferData.light.SpecularPower = m_Light->GetSpecularPower();

		BufferData.camera.CameraPosition = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(SceneManager->GetObjectManager()->GetCamera()->GetPosition());

		m_ShaderManager->UpdateBuffer(m_D3D->GetDeviceContext(), BufferData);
	}

	// 렌더링 //
	Render(SceneManager
#ifdef _DEBUG
		, input
#endif // _DEBUG
	);
}

void Graphic::GraphicsClass::Initialize(int ScreenWidth, int ScreenHeight)
{
	DirectX::XMFLOAT4 AmbientColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 DiffuseColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	DirectX::XMFLOAT4 LightDirection = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	DirectX::XMFLOAT4 SpecularColor = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float SpecularPower = 64.f;

	// Direct3D 객체 생성 및 초기화 //
	m_D3D = std::make_unique<D3DClass>(ScreenWidth, ScreenHeight);
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

	// terrain 객체 생성 //
	m_Terrain = std::make_unique<Terrain::TerrainClass>();
	assert(m_Terrain);

#ifdef _DEBUG
	// IMGUI 객체 생성 //
	m_IMGUI = std::make_unique<IMGUIClass>(m_D3D->GetDevice(), m_D3D->GetDeviceContext());
	assert(m_IMGUI);
#endif // DEBUG
}

void Graphic::GraphicsClass::Load(Scene::SceneManagerClass* SceneManager)
{
	UINT ShaderMask = 0;

	m_ModelManager->Load(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetObjectManager()->GetModelMask());
	m_UITextureManager->Load(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetUIManager()->GetUITextureMask());
	m_UIRender->LoadFont(m_D3D->GetDevice(), SceneManager->GetTextManager()->GetFontMask());
	m_Terrain->Load(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), SceneManager->GetTerrainID(), SceneManager->GetSkyDomeID());

	ShaderMask = m_ModelManager->GetNeedShaderMask();
	if (SceneManager->GetTerrainID() != Graphic::Terrain::TerrainID::NONE)
		ShaderMask |= (1 << static_cast<UINT>(m_Terrain->GetShaderID()));
	m_ShaderManager->Load(m_D3D->GetDevice(), ShaderMask);
}

void Graphic::GraphicsClass::Render(Scene::SceneManagerClass* SceneManager
#ifdef _DEBUG
	, System::InputClass* input
#endif // _DEBUG
)
{
	Model::IModelClass* model = nullptr;
	Shader::IShaderClass* shader = nullptr;
	UI::IUIClass* ui = nullptr;
	Text::ITextClass* text = nullptr;
	Model::ID id = Model::ID::NONE;
	UINT icnt = 0, jcnt = 0;

	// front buffer 초기화 //
	m_D3D->BeginScene(DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f));

	if (SceneManager->GetObjectManager()->GetCamera())
	{
		// terrain 렌더링 //
		shader = m_ShaderManager->GetShader(m_Terrain->GetShaderID());
		m_Terrain->Render(m_D3D->GetDeviceContext(), m_D3D.get(), shader, SceneManager->GetObjectManager()->GetCamera(), m_D3D->GetProjectionMatrix());

		// 3D 물체 렌더링 //
		icnt = m_ModelManager->GetCurrentLoadedModelIDCount();
		for (UINT i = 0; i < icnt; ++i)
		{
			// model 가져오기
			id = m_ModelManager->GetModelID(i);
			model = m_ModelManager->GetModel(id);
			assert(model);

			// shader, input layout 세팅
			shader = m_ShaderManager->GetShader(model->GetShaderID());
			shader->BeginRender(m_D3D->GetDeviceContext());

			jcnt = model->GetMeshCount();
			for (UINT j = 0; j < jcnt; ++j)
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
	icnt = SceneManager->GetUIManager()->GetUICount();
	for (UINT i = 0; i < icnt; ++i)
	{
		// UI 가져오기
		ui = SceneManager->GetUIManager()->GetUI(i);
		assert(ui);
		
		// 현재 UI가 보인다면 렌더링
		if (ui->GetUIState() & (1 << static_cast<UINT>(UI::UIState::APPEAR)))
		{
			// background를 제외한 보이는 모든 UI 렌더링
			if (UI::ID::BACKGROUND != ui->GetUIID())
			{
				m_UIRender->RenderTexture(
					m_UITextureManager->GetTexture(ui->GetUITextureID()),
					ui->GetPosition(),
					ui->GetColor(),
					ui->GetRotation(),
					ui->GetOrigin(),
					ui->GetScale(),
					ui->GetDepth()
				);
			}
			// background 렌더링
			else
				m_UIRender->RenderBackground(m_UITextureManager->GetTexture(ui->GetUITextureID()), ui->GetColor(), ui->GetDepth());
		}
	}

	// text 렌더링
	icnt = SceneManager->GetTextManager()->GetTextCount();
	for (UINT i = 0; i < icnt; ++i)
	{
		// text 가져오기
		text = SceneManager->GetTextManager()->GetTextObject(i);
		assert(text);

		// 현재 text가 보인다면 렌더링
		if(text->GetTextState() & (1 << static_cast<UINT>(UI::UIState::APPEAR)))
			m_UIRender->RenderText(
				text->GetText(),
				text->GetFontID(),
				text->GetPosition(),
				text->GetColor(),
				text->GetRotation(),
				text->GetOrigin(),
				text->GetScale(),
				text->GetDepth()
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


