#include "pch.h"
#include "TypeConverter.h"
#include "ICollisionContorlClass.h"

ICollisionContorlClass::ICollisionContorlClass(const AffineInfo& affine) : IAffineControlClass(affine) {}

const DirectX::ContainmentType& ICollisionContorlClass::GetCollideStateBetweenOBBAndOBB(const AffineInfo& affine)
{
	// 자신의 OBB 박스 생성 //
	DirectX::BoundingOrientedBox itself(TypeConverter::XMFLOAT4toXMFLOAT3(GetPosition()),
		TypeConverter::XMFLOAT4toXMFLOAT3(GetScaling()),
		GetRotation());

	// 충돌 검사 대상의 OBB 박스 생성 //
	DirectX::BoundingOrientedBox other(TypeConverter::XMFLOAT4toXMFLOAT3(affine.position),
		TypeConverter::XMFLOAT4toXMFLOAT3(affine.scale),
		affine.rotation);

	// 충돌 검사 //
	DirectX::ContainmentType ret = itself.Contains(other);

	// 충돌 검사 후, 결과에 따라 자신의 collision 색상 업데이트 //
	if(ret != DirectX::ContainmentType::DISJOINT)
		m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 0.f);
	else
		m_Color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 0.f);

	return ret;
}

const DirectX::ContainmentType& ICollisionContorlClass::GetCollideStateBetweenRayAndOBB(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& forward, float& CollideDistance)
{
	DirectX::BoundingOrientedBox itself;
	DirectX::XMVECTOR origin;
	DirectX::XMVECTOR vector;
	bool ret;

	// ray의 forward vector 검사
	origin = DirectX::XMLoadFloat4(&position);
	vector = DirectX::XMLoadFloat4(&forward);
	if (DirectX::XMVector3Equal(vector, DirectX::XMVectorZero()))
		return DirectX::ContainmentType::DISJOINT;

	// 자신의 OBB 박스 설정 //
	itself.Center = TypeConverter::XMFLOAT4toXMFLOAT3(GetPosition());
	itself.Extents = TypeConverter::XMFLOAT4toXMFLOAT3(GetScaling());
	itself.Orientation = GetRotation();

	// 충돌 검사 //
	ret = itself.Intersects(origin, vector, CollideDistance);

	// 충돌 검사 후, 결과에 따라 자신의 collision 색상 업데이트 //
	if (ret)
		m_Color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 0.f);
	else
		m_Color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 0.f);

	return (ret ? DirectX::ContainmentType::INTERSECTS : DirectX::ContainmentType::DISJOINT);
}