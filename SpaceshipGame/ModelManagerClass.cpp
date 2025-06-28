#include "pch.h"
#include "ModelClass.h"
#include "CubeModelClass.h"
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

	// model 객체 생성 및 map에 insert //
	model = std::make_unique<ModelClass<PTN_VertexType>>(hwnd, Device, DeviceContext, ID::DEFAULT_SPACESHIP);
	assert(model);
	m_ModelList.insert(std::make_pair(Graphic::Model::ID::DEFAULT_SPACESHIP, std::move(model)));

	model = new CubeModelClass(hwnd, Device, DeviceContext, CubeModelInfo, DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	assert(model);
	m_ModelList.insert(std::make_pair(Graphic::Model::ID::COLLISION, model));

	return result;
}

void Graphic::Model::ModelManagerClass::Shutdown()
{
	m_ModelList.clear();
}
