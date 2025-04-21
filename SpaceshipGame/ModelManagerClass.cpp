#include "pch.h"
#include "SpaceshipModelClass.h"
#include "CubeModelClass.h"
#include "ModelManagerClass.h"

bool ModelManagerClass::IsInitialize = false;
static ErrorContent e;

ModelManagerClass::ModelManagerClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
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

	result = Initailize(hwnd, Device, DeviceContext);
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

ModelClass* const ModelManagerClass::GetModel(ModelIDs key)
{
	ModelClass* ret = nullptr;
	std::map<ModelIDs, ModelClass*>::iterator iter;
	
	iter = m_ModelList.find(key);
	if (m_ModelList.end() == iter)
		return ret;

	ret = iter->second;
	return ret;
}

HRESULT ModelManagerClass::Initailize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;
	ModelClass* model = nullptr;

	// 에러 메세지 초기화 //
	e.title = _T("ModelManagerClass Initailize()");

	// model 객체 생성 및 map에 insert //
	model = new SpaceshipModelClass(hwnd, Device, DeviceContext, SpaceshipModelInfo);
	if (!model)
	{
		e.contents = _T("Model 인스턴스 생성 실패(spaceship)");
		e.errorCode = E_FAIL;
		return result;
	}
	m_ModelList.insert(std::make_pair(ModelIDs::DEFAULT_SPACESHIP, model));

	model = new CubeModelClass(hwnd, Device, DeviceContext, CubeModelInfo, DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
	if (!model)
	{
		e.contents = _T("Model 인스턴스 생성 실패(cube)");
		e.errorCode = E_FAIL;
		return result;
	}
	m_ModelList.insert(std::make_pair(ModelIDs::DEFAULT_CUBE, model));

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

	m_ModelList.clear();
}
