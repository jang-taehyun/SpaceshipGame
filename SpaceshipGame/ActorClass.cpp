#include "pch.h"
#include "TransformClass.h"
#include "ActorClass.h"

static ErrorContent e;

ActorClass::ActorClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, const int ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass constructor");

	result = Initailize(position, rotation, scaling, ModelID);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

ActorClass::~ActorClass()
{
	Shutdown();
}

inline const DirectX::XMMATRIX& ActorClass::GetAffineMatrix() const
{
	return m_Transform->GetAffine();
}

HRESULT ActorClass::Initailize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, const int ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass Initailize()");

	// transform 인스턴스 생성 //
	m_Transform = new TransformClass(position, rotation, scaling);
	if (!m_Transform)
	{
		e.contents = _T("transform 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_ModelID = ModelID;

	return result;
}

void ActorClass::Shutdown()
{
	if (m_Transform)
	{
		delete m_Transform;
		m_Transform = nullptr;
	}
}
