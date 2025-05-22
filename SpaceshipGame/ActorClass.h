#pragma once

/**
* ActorClass 개요
* - actor의 affine과 관련된 interface 관리
* - actor의 collision과 관련된 interface 관리
*/

class IAffineControlClass;

class ActorClass
{
public:
	ActorClass(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~ActorClass();

	inline IAffineControlClass* const& GetAffineInterface() const { return m_AffineInterface; }
	inline IAffineControlClass* const& GetCollisionInterface() const { return m_CollisionInterface; }
	inline const ModelIDs& GetModelID() const { return m_ModelID; }

private:
	HRESULT Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	void Shutdown();

private:
	IAffineControlClass* m_AffineInterface = nullptr;			// actor의 affine 관련 인터페이스
	IAffineControlClass* m_CollisionInterface = nullptr;		// collision 관련 인터페이스
	ModelIDs m_ModelID = ModelIDs::DEFAULT_SPACESHIP;

public:
	ActorClass() = delete;
	ActorClass(const ActorClass& other) = delete;
};

