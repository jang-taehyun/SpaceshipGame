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
	inline const DirectX::XMFLOAT4& GetColor() const { return m_Color; }

	void SetCenter(const DirectX::XMFLOAT3& center);
	void SetRotate(const DirectX::XMFLOAT4& quat);
	void SetExtents(const DirectX::XMFLOAT3& extents);
	inline void SetColor(const DirectX::XMFLOAT4& value) { m_Color = value; }

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

