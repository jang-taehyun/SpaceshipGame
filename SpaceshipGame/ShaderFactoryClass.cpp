#include "pch.h"
#include "MLC_ShaderClass.h"
#include "ShaderFactoryClass.h"

bool Graphic::Loader::ShaderFactoryClass::IsInitialize = false;

Graphic::Loader::ShaderFactoryClass::ShaderFactoryClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

	m_Creator.insert(std::make_pair(Shader::ID::DEFAULT_SPACESHIP, CreateDefaultSpaceshipShader));
	m_Creator.insert(std::make_pair(Shader::ID::CUBE, CreateCubeShader));
}

Graphic::Loader::ShaderFactoryClass::~ShaderFactoryClass()
{
	IsInitialize = false;
}

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::ShaderFactoryClass::Load(HWND hwnd, ID3D11Device* Device, Shader::ID id) const
{
	std::map<Shader::ID, std::function<std::unique_ptr<Shader::IShaderClass>(HWND, ID3D11Device*, Shader::ID)>>::const_iterator iter;

	iter = m_Creator.find(id);
	assert(m_Creator.end() != iter);

	return iter->second(hwnd, Device, id);
}

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::CreateDefaultSpaceshipShader(HWND hwnd, ID3D11Device* Device, Shader::ID id)
{
	// shader 정보 설정
	const ShaderFileInfo info =
	{
		_T("./shader/vertex/Spaceship.vs"),
		_T("./shader/pixel/Spaceship.ps"),
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
	shader->Initialize(hwnd, Device, info, semantics);

	return shader;
}

std::unique_ptr<Graphic::Shader::IShaderClass> Graphic::Loader::CreateCubeShader(HWND hwnd, ID3D11Device* Device, Shader::ID id)
{
	// shader 정보 설정
	const ShaderFileInfo info =
	{
		_T("./shader/vertex/color.vs"),
		_T("./shader/pixel/color.ps"),
		"ColorVertexShader",
		"ColorPixelShader"
	};

	// semantic 정보 설정
	std::vector<std::string> semantics =
	{
		"POSITION",
	};

	// instance 생성
	std::unique_ptr<Shader::IShaderClass> shader = std::make_unique<Shader::>(id);
	assert(shader);

	// instance 초기화
	shader->Initialize(hwnd, Device, info, semantics);

	return shader;
}
