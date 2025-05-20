#pragma once

class ICollisionContorlClass;
class IAffineControlClass;

class ActorClass
{
public:
	explicit ActorClass(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~ActorClass();

	inline IAffineControlClass* const& GetAffine() const { return m_AffineInterface; }
	inline ICollisionContorlClass* const& GetCollision() const { return m_CollisionInterface; }
	inline const ModelIDs& GetModelID() const { return m_ModelID; }

private:
	HRESULT Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	void Shutdown();

private:
	IAffineControlClass* m_AffineInterface = nullptr;
	ICollisionContorlClass* m_CollisionInterface = nullptr;
	ModelIDs m_ModelID = ModelIDs::DEFAULT_SPACESHIP;

public:
	ActorClass() = delete;
	ActorClass(const ActorClass& other) = delete;
};

