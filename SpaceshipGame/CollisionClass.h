#pragma once

#include <SimpleMath.h>

class CollisionClass
{
public:
	explicit CollisionClass(const DirectX::XMFLOAT3& dist);
	virtual ~CollisionClass();

	inline DirectX::BoundingBox* const& GetCollision() { return m_Collision; }
	const DirectX::ContainmentType GetCollideState(CollisionClass* const& ref);

private:
	HRESULT Initialize(const DirectX::XMFLOAT3& dist);
	void Shutdown();

private:
	DirectX::BoundingBox* m_Collision = nullptr;

public:
	CollisionClass() = delete;
	CollisionClass(const CollisionClass& other) = delete;
};

