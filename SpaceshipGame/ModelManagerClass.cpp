#include "pch.h"
#include "ModelClass.h"
#include "ModelManagerClass.h"

bool ModelManagerClass::IsInitialize = false;
static ErrorContent e;

ModelManagerClass::ModelManagerClass(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ModelManagerClass constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 ModelManagerClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initailize(Device, DeviceContext);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}

	IsInitialize = true;
}

ModelManagerClass::~ModelManagerClass()
{
	Shutdown();
	IsInitialize = false;
}

ModelClass* const& ModelManagerClass::GetModel(ModelIDs key)
{
	std::map<ModelIDs, ModelClass*>::iterator iter;
	
	iter = m_ModelList.find(key);
	if (m_ModelList.end() == iter)
		return nullptr;
	return iter->second;
}

HRESULT ModelManagerClass::Initailize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;
	ModelClass* model = nullptr;
	float ScalingFactor = 0.5f;
	DirectX::XMFLOAT4 Position = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 Rotation = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 Scaling = { ScalingFactor, ScalingFactor, ScalingFactor, 1.f };

	// 에러 메세지 초기화 //
	e.title = _T("ModelManagerClass Initailize()");

	// model 객체 생성 및 map에 insert //
	model = new ModelClass(Position, Rotation, Scaling, Device, DeviceContext, CubeTextureFileNames, CubeModelFileName);
	if (!model)
	{
		e.contents = _T("Model 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return result;
	}
	m_ModelList.insert(std::make_pair(ModelIDs::DEFAULT_SPACESHIP, model));

	return result;
}

void ModelManagerClass::Shutdown()
{
	std::map<ModelIDs, ModelClass*>::iterator iter;

	for (iter = m_ModelList.begin(); iter != m_ModelList.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}
}
