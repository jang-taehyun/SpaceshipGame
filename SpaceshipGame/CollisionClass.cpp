#include "pch.h"
#include "CollisionClass.h"

static ErrorContent e;

CollisionClass::CollisionClass(const DirectX::XMFLOAT3& dist)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CollisionClass constructor");

	result = Initialize(dist);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

CollisionClass::~CollisionClass()
{
	Shutdown();
}

const DirectX::ContainmentType CollisionClass::GetCollideState(CollisionClass* const& ref)
{
	DirectX::ContainmentType ret = m_Collision->Contains(*ref->GetCollision());
	return ret;
}

HRESULT CollisionClass::Initialize(const DirectX::XMFLOAT3& dist)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CollisionClass Initialize()");

	m_Collision = new DirectX::BoundingBox;
	if (!m_Collision)
	{
		e.contents = _T("bounding box 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_Collision->Extents = dist;

	return result;
}

void CollisionClass::Shutdown()
{
	if (m_Collision)
	{
		delete m_Collision;
		m_Collision = nullptr;
	}
}
