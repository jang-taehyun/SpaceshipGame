#include "pch.h"
#include "TypeConverterClass.h"
#include "CollisionClass.h"

Object::CollisionClass::CollisionClass(const AffineInfo& affine, float range) : ObjectClass(affine), m_Range(range) {}

DirectX::ContainmentType Object::CollisionClass::GetCollideState(const AffineInfo& affine)
{
	// 자신의 OBB 박스 생성 //
	DirectX::BoundingOrientedBox itself(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(GetPosition()),
		Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(GetScale()),
		GetRotation());

	// 충돌 검사 대상의 OBB 박스 생성 //
	DirectX::BoundingOrientedBox other(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(affine.position),
		Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(affine.scale),
		affine.rotation);

	// 충돌 검사 //
	DirectX::ContainmentType ret = itself.Contains(other);

#ifdef DEBUG
	// 충돌 검사 후, 결과에 따라 자신의 collision 색상 업데이트 //
	if (ret != DirectX::ContainmentType::DISJOINT)
		m_Color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	else
		m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);
#endif // DEBUG

	return ret;
}

DirectX::ContainmentType Object::CollisionClass::GetCollideState(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 forward)
{
	DirectX::BoundingOrientedBox itself;
	DirectX::XMVECTOR origin;
	DirectX::XMVECTOR vector;
	float dist = 0.f;
	bool result = false;
	DirectX::ContainmentType ret;

	// ray의 forward vector 검사
	vector = DirectX::XMLoadFloat4(&forward);
	assert(DirectX::XMVector3Equal(vector, DirectX::XMVectorZero()));
	origin = DirectX::XMLoadFloat4(&position);

	// 자신의 OBB 박스 설정 //
	itself.Center = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(GetPosition());
	itself.Extents = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(GetScale());
	itself.Orientation = GetRotation();

	// 충돌 검사 //
	result = itself.Intersects(origin, vector, dist);

	// 충돌 검사 후, 결과에 따라 자신의 collision 색상 업데이트 //
	if (result && (dist <= m_Range))
		ret = DirectX::ContainmentType::INTERSECTS;
	else
		ret = DirectX::ContainmentType::DISJOINT;

#ifdef DEBUG
	m_Color = (ret == DirectX::ContainmentType::INTERSECTS ? DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f) : DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f));
#endif // DEBUG

	return ret;
}
