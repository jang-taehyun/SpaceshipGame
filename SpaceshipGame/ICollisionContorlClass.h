#pragma once

/**
* ICollisionContorlClass 개요
* - 충돌 검사
* - 충돌체 색상 관리
*/

#include <DirectXCollision.h>

#include "IAffineControlClass.h"

class ICollisionContorlClass : public IAffineControlClass
{
public:
	explicit ICollisionContorlClass(const AffineInfo& affine);
	virtual ~ICollisionContorlClass() = default;

	const DirectX::XMFLOAT4& GetCollisionColor() const { return m_Color; }
	void SetCollisionColor(const DirectX::XMFLOAT4& color) { m_Color = color; }

	const DirectX::ContainmentType& GetCollideStateBetweenOBBAndOBB(const AffineInfo& affine);
	const DirectX::ContainmentType& GetCollideStateBetweenRayAndOBB(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& forward, float& CollideDistance);

private:
	DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 0.f);								// collision 색상

public:
	ICollisionContorlClass() = delete;
	ICollisionContorlClass(const ICollisionContorlClass& other) = delete;
};

