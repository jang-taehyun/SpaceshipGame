#pragma once

/**
* CollisionClass 개요
* - 충돌체의 색상 관리
* - 충돌체 인스턴스 관리
*/

#include <SimpleMath.h>
#include <DirectXCollision.h>

class CollisionClass
{
public:
	explicit CollisionClass(const AffineInfo& affine);
	virtual ~CollisionClass();

	inline const DirectX::BoundingOrientedBox* const& GetCollisionBox() const { return m_Collision; }
	inline const DirectX::SimpleMath::Ray* const& GetRay() const { return m_Ray; }

	inline const DirectX::XMFLOAT4& GetColor() const { return m_Color; }
	inline void SetColor(const DirectX::XMFLOAT4& value) { m_Color = value; }
	void SetColor(const float& red, const float& green, const float& blue, const float& alpha);

	HRESULT CreateRay();

private:
	HRESULT Initialize(const AffineInfo& affine);
	void Shutdown();

private:
	DirectX::BoundingOrientedBox* m_Collision = nullptr;
	DirectX::SimpleMath::Ray* m_Ray = nullptr;
	DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);

public:
	CollisionClass(const CollisionClass& other) = delete;
};

