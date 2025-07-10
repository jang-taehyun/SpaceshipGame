#include "pch.h"
#include "ModelClass.hpp"
#include "PTN_DNSR_ModelLoaderClass.h"
#include "PTN_ModelLoaderClass.h"
#include "ModelFactoryClass.h"

bool Graphic::Loader::ModelFactoryClass::IsInitialize = false;

Graphic::Loader::ModelFactoryClass::ModelFactoryClass()
{
	m_Creator.insert(std::make_pair(Model::ID::COLLISION, LoadCollision));

	m_Creator.insert(std::make_pair(Model::ID::DEFAULT_SPACESHIP, LoadDefaultSpaceship));

	IsInitialize = true;
}

Graphic::Loader::ModelFactoryClass::~ModelFactoryClass()
{
	IsInitialize = false;
}

std::unique_ptr<Graphic::Model::IModelClass> Graphic::Loader::ModelFactoryClass::Load(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Model::ID ModelID) const
{
	std::map<Model::ID, std::function<std::unique_ptr<Model::IModelClass>(HWND, ID3D11Device*, ID3D11DeviceContext*)>>::const_iterator iter;

	iter = m_Creator.find(ModelID);
	assert(m_Creator.end() != iter);

	return iter->second(hwnd, Device, DeviceContext);
}

std::unique_ptr<Graphic::Model::IModelClass> Graphic::Loader::LoadDefaultSpaceship(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	std::unique_ptr<Model::IModelClass> model = nullptr;
	std::unique_ptr<Loader::IModelLoaderClass> loader = nullptr;

	loader = std::make_unique<Loader::PTN_DNSR_ModelLoaderClass>(Model::ID::DEFAULT_SPACESHIP);
	assert(loader);
	loader->Load(Device, DeviceContext);

	model = std::make_unique<Model::ModelClass<Model::PTN_VertexType>>(hwnd, Device, DeviceContext, Model::ID::DEFAULT_SPACESHIP, Shader::ID::DEFAULT_SPACESHIP, loader.get());
	assert(model);

	return model;
}

std::unique_ptr<Graphic::Model::IModelClass> Graphic::Loader::LoadCollision(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	std::unique_ptr<Model::IModelClass> model = nullptr;
	std::unique_ptr<Loader::IModelLoaderClass> loader = nullptr;

	loader = std::make_unique<Loader::PTN_ModelLoaderClass>(Model::ID::COLLISION);
	assert(loader);
	loader->Load(Device, DeviceContext);

	model = std::make_unique<Model::ModelClass<Model::PTN_VertexType>>(hwnd, Device, DeviceContext, Model::ID::COLLISION, Shader::ID::CUBE, loader.get());
	assert(model);

	return model;
}
