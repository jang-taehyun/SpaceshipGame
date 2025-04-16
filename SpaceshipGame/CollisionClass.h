#pragma once

#include <SimpleMath.h>

class CollisionClass
{
public:
	CollisionClass(const DirectX::XMFLOAT3& dist);
	~CollisionClass();

	inline DirectX::BoundingBox* const& GetCollision() { return m_Collision; }
	const DirectX::ContainmentType GetCollideState(CollisionClass* const& ref);

private:
	HRESULT Initialize(const DirectX::XMFLOAT3& dist);
	void Shutdown();

private:
	DirectX::BoundingBox* m_Collision = nullptr;
};

