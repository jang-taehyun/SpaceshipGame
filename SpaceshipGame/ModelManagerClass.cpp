#include "pch.h"
#include "ModelClass.h"
#include "PTN_DNSR_ModelLoaderClass.h"
#include "PTN_ModelLoaderClass.h"
#include "ModelManagerClass.h"

bool Graphic::Model::ModelManagerClass::IsInitialize = false;

Graphic::Model::ModelManagerClass::ModelManagerClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	assert(IsInitialize);

	HRESULT result = Initailize(hwnd, Device, DeviceContext);
	IsInitialize = true;
}

Graphic::Model::ModelManagerClass::~ModelManagerClass()
{
	Shutdown();
	IsInitialize = false;
}

const Graphic::Model::IModelClass* Graphic::Model::ModelManagerClass::GetModel(Graphic::Model::ID key) const
{
	std::map<Graphic::Model::ID, std::unique_ptr<IModelClass>>::const_iterator iter;
	
	iter = m_ModelList.find(key);
	if (m_ModelList.end() == iter)
		return nullptr;

	return iter->second.get();
}

HRESULT Graphic::Model::ModelManagerClass::Initailize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	HRESULT result = S_OK;
	std::unique_ptr<IModelClass> model = nullptr;
	std::unique_ptr<Loader::IModelLoaderClass> loader = nullptr;

	// 扁夯 快林急 葛胆 按眉 积己 //
	// model 按眉 积己
	loader = std::make_unique<Loader::PTN_DNSR_ModelLoaderClass>(ID::DEFAULT_SPACESHIP);
	model = std::make_unique<ModelClass<PTN_VertexType>>(hwnd, Device, DeviceContext, ID::DEFAULT_SPACESHIP, loader);
	assert(model);

	// map俊 insert
	m_ModelList.insert(std::make_pair(Graphic::Model::ID::DEFAULT_SPACESHIP, std::move(model)));
	loader.reset();

	// cube 葛胆 按眉 积己 //
	// model 按眉 积己
	loader = std::make_unique<Loader::PTN_ModelLoaderClass>(ID::COLLISION);
	model = std::make_unique<ModelClass<PTN_VertexType>>(hwnd, Device, DeviceContext, ID::COLLISION, loader);
	assert(model);

	// map俊 insert
	m_ModelList.insert(std::make_pair(Graphic::Model::ID::COLLISION, model));
	loader.reset();

	return result;
}

void Graphic::Model::ModelManagerClass::Shutdown()
{
	m_ModelList.clear();
}
