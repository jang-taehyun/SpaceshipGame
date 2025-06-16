#pragma once

/**
* ActorManagerClass °³¿ä
*/

class IObjectClass;

const static int ACTOR_MAX_COUNT = 6;

class ActorManagerClass
{
public:
	ActorManagerClass(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const ModelIDs* ModelIDs, int ActorCount, int PlayerIdx);
	virtual ~ActorManagerClass();

	inline const IObjectClass* GetPlayerInterface() const { return m_ActorInterfaces[m_PlayerIdx].get(); }
	inline int GetActorInterfaceCount() const { return m_ActorCount; }

	const IObjectClass* operator[](int idx) const;

private:
	HRESULT Initialize(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const ModelIDs* ModelIDs);

private:
	static bool IsInitialize;

	std::unique_ptr<IObjectClass> m_ActorInterfaces[ACTOR_MAX_COUNT] = { nullptr, };
	int m_ActorCount = 0;
	int m_PlayerIdx = 0;

public:
	ActorManagerClass(const ActorManagerClass& other) = delete;
	ActorManagerClass(ActorManagerClass&& other) = delete;
	ActorManagerClass& operator=(const ActorManagerClass& other) = delete;
	ActorManagerClass& operator=(ActorManagerClass&& other) = delete;
};

