#pragma once

#include "AffineClass.h"

class CollisionClass;
class ModelClass;

class ActorClass
{
public:
	explicit ActorClass(const AffineInfo& affine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~ActorClass();

	inline const DirectX::XMMATRIX& GetAffineMatrix() { return m_Affine->GetAffine(); }
	inline CollisionClass* const& GetCollision() { return m_Collision; }
	inline const ModelIDs& GetModelID() { return m_ModelID; }

	inline AffineClass* const& GetAffineObject() const { return m_Affine; }

private:
	HRESULT Initailize(const AffineInfo& affine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	void Shutdown();

private:
	AffineClass* m_Affine = nullptr;
	CollisionClass* m_Collision = nullptr;
	ModelIDs m_ModelID = ModelIDs::DEFAULT_SPACESHIP;

public:
	ActorClass() = delete;
	ActorClass(const ActorClass& other) = delete;
};

