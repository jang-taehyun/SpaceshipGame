#include "pch.h"
#include <SimpleMath.h>

#include "CollisionClass.h"
#include "ActorClass.h"

static ErrorContent e;

ActorClass::ActorClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, const ModelIDs ModelID)
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

HRESULT ActorClass::Initailize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling, const ModelIDs ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass Initailize()");

	// transform 인스턴스 생성 //
	m_Affine = new AffineClass(position, rotation, scaling);
	if (!m_Affine)
	{
		e.contents = _T("AffineClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_ModelID = ModelID;

	return result;
}

void ActorClass::Shutdown()
{
	if (m_Collision)
	{
		delete m_Collision;
		m_Collision = nullptr;
	}

	if (m_Affine)
	{
		delete m_Affine;
		m_Affine = nullptr;
	}
}