#include "pch.h"
#include "CollisionClass.h"

static ErrorContent e;

CollisionClass::CollisionClass(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CollisionClass constructor");

	result = Initialize(affine);
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

void CollisionClass::SetColor(const float& red, const float& green, const float& blue, const float& alpha)
{
	m_Color.x = red;
	m_Color.y = green;
	m_Color.z = blue;
	m_Color.w = alpha;
}

HRESULT CollisionClass::CreateRay()
{
	HRESULT result = S_OK;
	DirectX::SimpleMath::Vector3 pos, dir;

	// ray가 이미 생성되어 있는지 검사 //
	if (m_Ray)
		return result;

	// 변수 초기화 //
	pos = m_Collision->Center;

	dir.x = m_Collision->Orientation.x;
	dir.y = m_Collision->Orientation.y;
	dir.z = m_Collision->Orientation.z;

	// ray 객체 생성 및 초기화 //
	m_Ray = new DirectX::SimpleMath::Ray;
	if (!m_Ray)
	{
		e.contents = _T("DirectX::SimpleMath::Ray 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}
	m_Ray->position = pos;
	m_Ray->direction = dir;

	return result;
}

HRESULT CollisionClass::Initialize(const AffineInfo& affine)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CollisionClass Initialize()");

	// collsion 인스턴스 생성 //
	m_Collision = new DirectX::BoundingOrientedBox;
	if (!m_Collision)
	{
		e.contents = _T("bounding oriented box 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}
	m_Collision->Center = DirectX::XMFLOAT3(affine.position.x, affine.position.y, affine.position.z);
	m_Collision->Orientation = affine.rotation;
	m_Collision->Extents = DirectX::XMFLOAT3(affine.scale.x, affine.scale.y, affine.scale.z);

	// collision의 색상 초기화 //
	m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	return result;
}

void CollisionClass::Shutdown()
{
	if (m_Ray)
	{
		delete m_Ray;
		m_Ray = nullptr;
	}

	if (m_Collision)
	{
		delete m_Collision;
		m_Collision = nullptr;
	}
}
