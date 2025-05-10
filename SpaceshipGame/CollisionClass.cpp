#include "pch.h"
#include "AffineClass.h"
#include "CollisionClass.h"

static ErrorContent e;

CollisionClass::CollisionClass()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CollisionClass constructor");

	result = Initialize();
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

const DirectX::ContainmentType CollisionClass::GetCollideState(DirectX::BoundingOrientedBox* const& collision)
{
	DirectX::ContainmentType ret = m_Collision->Contains(*collision);
	return ret;
}

const DirectX::ContainmentType CollisionClass::GetCollideState(const DirectX::SimpleMath::Ray* const& ray)
{
	if (!m_Collision || !ray)
		return DirectX::ContainmentType::DISJOINT;

	float dist = 5.f;
	DirectX::XMVECTOR origin = XMLoadFloat3(&ray->position);
	DirectX::XMVECTOR direction = XMLoadFloat3(&ray->direction);
	bool IsHit = false;

	if (DirectX::XMVector3Equal(direction, DirectX::XMVectorZero()))
		return DirectX::ContainmentType::DISJOINT;

	direction = DirectX::XMVector3Normalize(direction);
	IsHit = m_Collision->Intersects(origin, direction, dist);

	return (IsHit ? DirectX::ContainmentType::CONTAINS : DirectX::ContainmentType::DISJOINT);
}

void CollisionClass::SetCenter(const DirectX::XMFLOAT3& center)
{
	m_Collision->Center = center;
	UpdateAffine();
}

void CollisionClass::SetCenter(const DirectX::XMFLOAT4& center)
{
	m_Collision->Center.x = center.x;
	m_Collision->Center.y = center.y;
	m_Collision->Center.z = center.z;
	UpdateAffine();
}

void CollisionClass::SetCenter(const float& x, const float& y, const float& z)
{
	m_Collision->Center.x = x;
	m_Collision->Center.y = y;
	m_Collision->Center.z = z;
	UpdateAffine();
}

void CollisionClass::SetRotate(const DirectX::XMFLOAT4& quat)
{
	m_Collision->Orientation = quat;
	UpdateAffine();
}

void CollisionClass::SetRotate(const float& x, const float& y, const float& z)
{
	m_Collision->Orientation.x = x;
	m_Collision->Orientation.y = y;
	m_Collision->Orientation.z = z;
	UpdateAffine();
}

void CollisionClass::SetExtents(const DirectX::XMFLOAT3& extents)
{
	m_Collision->Extents = extents;
	UpdateAffine();
}

void CollisionClass::SetExtents(const DirectX::XMFLOAT4& extents)
{
	m_Collision->Extents.x = extents.x;
	m_Collision->Extents.y = extents.y;
	m_Collision->Extents.z = extents.z;
	UpdateAffine();
}

void CollisionClass::SetExtents(const float& x, const float& y, const float& z)
{
	m_Collision->Extents.x = x;
	m_Collision->Extents.y = y;
	m_Collision->Extents.z = z;
	UpdateAffine();
}

void CollisionClass::SetColor(const float& red, const float& green, const float& blue, const float& alpha)
{
	m_Color.x = red;
	m_Color.y = green;
	m_Color.z = blue;
	m_Color.w = alpha;
}

HRESULT CollisionClass::Initialize()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CollisionClass Initialize()");

	m_Collision = new DirectX::BoundingOrientedBox;
	if (!m_Collision)
	{
		e.contents = _T("bounding oriented box 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_Affine = new AffineClass();
	if (!m_Affine)
	{
		e.contents = _T("Affine 클래스 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	return result;
}

void CollisionClass::Shutdown()
{
	if (m_Affine)
	{
		delete m_Affine;
		m_Affine = nullptr;
	}

	if (m_Collision)
	{
		delete m_Collision;
		m_Collision = nullptr;
	}
}

void CollisionClass::UpdateAffine()
{
	// Extents
	DirectX::XMVECTOR scaleVec = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(m_Collision->Extents)), 1.0f);
	DirectX::XMFLOAT4 scaleF4;
	DirectX::XMStoreFloat4(&scaleF4, scaleVec);
	m_Affine->SetScale(scaleF4);

	// Orientation
	DirectX::XMVECTOR quat = DirectX::XMLoadFloat4(&(m_Collision->Orientation));
	DirectX::XMFLOAT4 quatF4;
	DirectX::XMStoreFloat4(&quatF4, quat);
	m_Affine->SetRotation(quatF4);

	// Center
	DirectX::XMVECTOR centerVec = XMLoadFloat3(&(m_Collision->Center));
	DirectX::XMFLOAT4 centerF4;
	DirectX::XMStoreFloat4(&centerF4, centerVec);
	m_Affine->SetPosition(centerF4);
}
