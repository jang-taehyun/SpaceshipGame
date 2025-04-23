#include "pch.h"
#include <SimpleMath.h>

#include "CollisionClass.h"
#include "ActorClass.h"

static ErrorContent e;

ActorClass::ActorClass(const AffineInfo& affine, const ModelIDs ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass constructor");

	result = Initailize(affine, ModelID);
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

HRESULT ActorClass::Initailize(const AffineInfo& affine, const ModelIDs ModelID)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ActorClass Initailize()");

	// affine 인스턴스 생성 //
	m_Affine = new AffineClass(affine);
	if (!m_Affine)
	{
		e.contents = _T("AffineClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_ModelID = ModelID;

	m_Collision = new CollisionClass();
	if (!m_Collision)
	{
		e.contents = _T("CollisionClass 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

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