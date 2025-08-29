#include "pch.h"
#include "MLC_ShaderClass.h"
#include "TerrainShaderClass.h"

#ifdef _DEBUG
#include "Cube_ShaderClass.h"
#endif // _DEBUG

#include "ShaderFactoryClass.h"

bool Graphic::Loader::ShaderFactoryClass::IsInitialize = false;

Graphic::Loader::ShaderFactoryClass::ShaderFactoryClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

#ifdef _DEBUG
	m_Creator.insert(std::make_pair(Shader::ID::CUBE, CreateCubeShader));
#endif // _DEBUG

	m_Creator.insert(std::make_pair(Shader::ID::DEFAULT_SPACESHIP, CreateDefaultSpaceshipShader));
	m_Creator.insert(std::make_pair(Shader::ID::TERRAIN, CreateDefaultTerrainShader));
}

Graphic::Loader::ShaderFactoryClass::~ShaderFactoryClass()
{
	IsInitialize = false;
	m_Creator.clear();
}

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::ShaderFactoryClass::Load(ID3D11Device* Device, Shader::ID id) const
{
	std::map<Shader::ID, std::function<std::unique_ptr<Shader::IShaderClass>(ID3D11Device*, Shader::ID)>>::const_iterator iter;

	iter = m_Creator.find(id);
	assert(m_Creator.end() != iter);

	return iter->second(Device, id);
}

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::CreateDefaultSpaceshipShader(ID3D11Device* Device, Shader::ID id)
{
	// shader 정보 설정
	const ShaderFileInfo info =
	{
		_T("./shader/vertex/Spaceship_vs.hlsl"),
		_T("./shader/pixel/Spaceship_ps.hlsl"),
		"SpaceshipVertexShader",
		"SpaceshipPixelShader"
	};

	// semantic 정보 설정
	std::vector<std::string> semantics =
	{
		"POSITION",
		"TEXCOORD",
		"NORMAL",
	};

	// instance 생성
	std::unique_ptr<Shader::IShaderClass> shader = std::make_unique<Shader::MLC_ShaderClass>(id);
	assert(shader);

	// instance 초기화
	shader->Initialize(Device, info, semantics);

	return shader;
}

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::CreateDefaultTerrainShader(ID3D11Device* Device, Shader::ID id)
{
	// shader 정보 설정
	const ShaderFileInfo info =
	{
		_T("./shader/vertex/terrain_vs.hlsl"),
		_T("./shader/pixel/terrain_ps.hlsl"),
		"TerrainVertexShader",
		"TerrainPixelShader"
	};

	// semantic 정보 설정
	std::vector<std::string> semantics =
	{
		"POSITION",
		"TEXCOORD",
		"NORMAL",
	};

	// instance 생성
	std::unique_ptr<Shader::IShaderClass> shader = std::make_unique<Shader::TerrainShaderClass>(id);
	assert(shader);

	// instance 초기화
	shader->Initialize(Device, info, semantics, true);

	return shader;
}

#ifdef _DEBUG

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::CreateCubeShader(ID3D11Device* Device, Shader::ID id)
{
	// shader 정보 설정
	const ShaderFileInfo info =
	{
		_T("./shader/vertex/color_vs.hlsl"),
		_T("./shader/pixel/color_ps.hlsl"),
		"ColorVertexShader",
		"ColorPixelShader"
	};

	// semantic 정보 설정
	std::vector<std::string> semantics =
	{
		"POSITION",
	};

	// instance 생성
	std::unique_ptr<Shader::IShaderClass> shader = std::make_unique<Shader::Cube_ShaderClass>(id);
	assert(shader);

	// instance 초기화
	shader->Initialize(Device, info, semantics);

	return shader;
}

#endif // _DEBUG