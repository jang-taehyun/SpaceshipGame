#include "pch.h"
#include "ModelClass.hpp"
#include "PTN_DNSR_ModelLoaderClass.h"
#include "PTN_ModelLoaderClass.h"
#include "ModelFactoryClass.h"

bool Graphic::Loader::ModelFactoryClass::IsInitialize = false;

Graphic::Loader::ModelFactoryClass::ModelFactoryClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

#ifdef _DEBUG
	m_Creator.insert(std::make_pair(Model::ID::COLLISION, LoadCollision));
#endif

	m_Creator.insert(std::make_pair(Model::ID::DEFAULT_SPACESHIP, LoadDefaultSpaceship));
}

Graphic::Loader::ModelFactoryClass::~ModelFactoryClass()
{
	IsInitialize = false;
	m_Creator.clear();
}

std::unique_ptr<Graphic::Model::IModelClass> Graphic::Loader::ModelFactoryClass::Load(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Model::ID ModelID) const
{
	std::map<Model::ID, std::function<std::unique_ptr<Model::IModelClass>(HWND, ID3D11Device*, ID3D11DeviceContext*, const std::string&, const std::wstring&)>>::const_iterator iter;
	std::map<Model::ID, const std::string>::const_iterator ModelFileIter;
	std::map<Model::ID, const std::wstring>::const_iterator AdditionalTexturePathIter;

	// ID를 통해 model 찾기
	ModelFileIter = m_ModelFileList.find(ModelID);
	assert(m_ModelFileList.end() != ModelFileIter);

	// ID를 통해 model의 texture의 추가적인 경로 찾기
	AdditionalTexturePathIter = m_ModelAdditionPathList.find(ModelID);
	assert(m_ModelAdditionPathList.end() != AdditionalTexturePathIter);

	// instance를 생성하는 함수 찾기
	iter = m_Creator.find(ModelID);
	assert(m_Creator.end() != iter);

	return iter->second(hwnd, Device, DeviceContext, ModelFileIter->second, AdditionalTexturePathIter->second);
}

std::unique_ptr<Graphic::Model::IModelClass> Graphic::Loader::LoadDefaultSpaceship(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath)
{
	std::unique_ptr<Model::IModelClass> model = nullptr;
	std::unique_ptr<Loader::IModelLoaderClass> loader = nullptr;
	
	loader = std::make_unique<Loader::PTN_DNSR_ModelLoaderClass>();
	assert(loader);
	loader->Load(Device, DeviceContext, ModelFilename, AdditionalPath);

	model = std::make_unique<Model::ModelClass<Model::PTN_VertexType>>(hwnd, Device, DeviceContext, Model::ID::DEFAULT_SPACESHIP, Shader::ID::DEFAULT_SPACESHIP, loader.get());
	assert(model);

	return model;
}

#ifdef _DEBUG

std::unique_ptr<Graphic::Model::IModelClass> Graphic::Loader::LoadCollision(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath)
{
	std::unique_ptr<Model::IModelClass> model = nullptr;
	std::unique_ptr<Loader::IModelLoaderClass> loader = nullptr;

	loader = std::make_unique<Loader::PTN_ModelLoaderClass>();
	assert(loader);
	loader->Load(Device, DeviceContext, ModelFilename, AdditionalPath);

	model = std::make_unique<Model::ModelClass<Model::PTN_VertexType>>(hwnd, Device, DeviceContext, Model::ID::COLLISION, Shader::ID::CUBE, loader.get());
	assert(model);

	return model;
}

#endif
