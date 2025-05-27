#pragma once

/**
* ActorClass °³¿ä
*/

#include "MoveableObjectClass.h"

class ActorClass : public MoveableObjectClass
{
public:
	ActorClass() = default;
	explicit ActorClass(ModelIDs m_ModelID);
	virtual ~ActorClass() = default;

	inline IAffineControlClass* const& GetAffineInterface() const { return m_AffineInterface; }
	inline IAffineControlClass* const& GetCollisionInterface() const { return m_CollisionInterface; }
	inline const ModelIDs& GetModelID() const { return m_ModelID; }

private:
	HRESULT Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	void Shutdown();

private:
	ModelIDs m_ModelID = ModelIDs::DEFAULT_SPACESHIP;
};

