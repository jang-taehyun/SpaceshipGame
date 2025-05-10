#pragma once

#include <SimpleMath.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

class AffineClass;

class CollisionClass
{
public:
	explicit CollisionClass();
	virtual ~CollisionClass();

	inline DirectX::BoundingOrientedBox* const& GetCollision() { return m_Collision; }
	const DirectX::ContainmentType GetCollideState(DirectX::BoundingOrientedBox* const& collision);
	const DirectX::ContainmentType GetCollideState(const DirectX::SimpleMath::Ray* const& ray);
	inline const DirectX::XMFLOAT4& GetColor() const { return m_Color; }
	inline AffineClass* const& GetAffineObject() const { return m_Affine; }

	void SetCenter(const DirectX::XMFLOAT3& center);
	void SetCenter(const DirectX::XMFLOAT4& center);
	void SetCenter(const float& x, const float& y, const float& z);
	void SetRotate(const DirectX::XMFLOAT4& quat);
	void SetRotate(const float& x, const float& y, const float& z);
	void SetExtents(const DirectX::XMFLOAT3& extents);
	void SetExtents(const DirectX::XMFLOAT4& extents);
	void SetExtents(const float& x, const float& y, const float& z);

	inline void SetColor(const DirectX::XMFLOAT4& value) { m_Color = value; }
	void SetColor(const float& red, const float& green, const float& blue, const float& alpha);

private:
	HRESULT Initialize();
	void Shutdown();

	void UpdateAffine();

private:
	DirectX::BoundingOrientedBox* m_Collision = nullptr;
	DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	AffineClass* m_Affine = nullptr;

public:
	CollisionClass(const CollisionClass& other) = delete;
};

