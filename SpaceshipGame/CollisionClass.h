#pragma once

#include <SimpleMath.h>
class AffineClass;

class CollisionClass
{
public:
	explicit CollisionClass();
	virtual ~CollisionClass();

	inline DirectX::BoundingOrientedBox* const& GetCollision() { return m_Collision; }
	const DirectX::ContainmentType GetCollideState(CollisionClass* const& ref);

	const DirectX::XMMATRIX& GetAffine() const;

	void SetCenter(const DirectX::XMFLOAT3& center);
	void SetRotate(const DirectX::XMFLOAT4& quat);
	void SetExtents(const DirectX::XMFLOAT3& extents);

private:
	HRESULT Initialize();
	void Shutdown();

	void UpdateAffine();

private:
	DirectX::BoundingOrientedBox* m_Collision = nullptr;
	AffineClass* m_Affine = nullptr;

public:
	CollisionClass(const CollisionClass& other) = delete;
};

