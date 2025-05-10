#pragma once

#include "AffineClass.h"

class CollisionClass;
class ModelClass;

class ActorClass
{
public:
	explicit ActorClass(const AffineInfo& affine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~ActorClass();

	inline AffineClass* const& GetAffineObject() const { return m_Affine; }
	inline CollisionClass* const& GetCollision() const { return m_Collision; }
	inline const ModelIDs& GetModelID() const { return m_ModelID; }

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

