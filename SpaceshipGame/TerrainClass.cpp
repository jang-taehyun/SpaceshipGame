#include "pch.h"
#include "QuadTreeClass.h"
#include "SkyDomeClass.h"
#include "TerrainShaderClass.h"
#include "CameraClass.h"
#include "TerrainClass.h"

bool Graphic::Terrain::TerrainClass::IsInitialize = false;

Graphic::Terrain::TerrainClass::TerrainClass()
{
	assert(!IsInitialize);
	IsInitialize = true;
}

Graphic::Terrain::TerrainClass::~TerrainClass()
{
	IsInitialize = false;
}

void Graphic::Terrain::TerrainClass::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, TerrainID terrain_id, SkyDomeID skydome_id)
{
	std::map<TerrainID, const char*>::iterator HeightmapIter;
	std::map<TerrainID, const WCHAR*>::iterator TerrainTextureIter;
	std::map<TerrainID, Shader::ID>::iterator ShaderIDIter;
	std::map<SkyDomeID, const WCHAR*>::iterator SkyDomeTextureIter;

	// 이전에 사용한 terrain, sky dome 해제
	m_QuadTree.reset();
	m_SkyDome.reset();
	m_TerrainShaderID = Shader::ID::NONE;

	// terrain 생성 //
	if (TerrainID::NONE != terrain_id)
	{
		// height map 경로 가져오기
		HeightmapIter = m_HeightMapList.find(terrain_id);
		assert(m_HeightMapList.end() != HeightmapIter);
	
		// terrain의 texture 경로 가져오기
		TerrainTextureIter = m_TerrainTextureList.find(terrain_id);
		assert(m_TerrainTextureList.end() != TerrainTextureIter);
	
		// terrain의 shader ID 경로 가져오기
		ShaderIDIter = m_TerrainShaderIDList.find(terrain_id);
		assert(m_TerrainShaderIDList.end() != ShaderIDIter);
	
		// terrain의 shader ID 설정
		m_TerrainShaderID = ShaderIDIter->second;
	
		// terrain 생성
		m_QuadTree = std::make_unique<QuadTreeClass>(Device, DeviceContext, HeightmapIter->second, TerrainTextureIter->second);
		assert(m_QuadTree);
	}
	
	// sky dome 생성 //
	if (SkyDomeID::NONE != skydome_id)
	{
		// sky dome의 texture 경로 가져오기
		SkyDomeTextureIter = m_SkyDomeTextureList.find(skydome_id);
	
		// sky dome 생성
		m_SkyDome = std::make_unique<SkyDomeClass>(Device, DeviceContext,
			(m_SkyDomeTextureList.end() != SkyDomeTextureIter ? SkyDomeTextureIter->second : nullptr)
		);
		assert(m_SkyDome);
	}
}

void Graphic::Terrain::TerrainClass::Render(ID3D11DeviceContext* DeviceContext, D3DClass* d3d, Shader::IShaderClass* TerrainShader, Object::IObjectClass* Camera, DirectX::XMFLOAT4X4 projection)
{
	Shader::TerrainShaderClass* shader = static_cast<Shader::TerrainShaderClass*>(TerrainShader);
	Object::CameraClass* c = static_cast<Object::CameraClass*>(Camera);

	// sky dome 렌더링
	if(m_SkyDome)
		m_SkyDome->Render(d3d, c->GetAffineMatrix(), c->GetViewMatrix(), projection);

	// terrain 렌더링
	if (m_QuadTree)
	{
		assert(shader);
		shader->BeginRender(DeviceContext);
		m_QuadTree->Render(DeviceContext, shader, c);
	}
}
