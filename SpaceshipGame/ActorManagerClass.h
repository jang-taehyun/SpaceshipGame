#pragma once

/**
* ActorManagerClass 개요
* - Scene 내에 존재하는 actor의 interface(behavior) 관리
*/

const static int ACTOR_MAX_COUNT = 6;

class IActorControlClass;

class ActorManagerClass
{
public:
	ActorManagerClass(const AffineInfo* const& ActorAffines, const AffineInfo* const& CollisionAffines, const ModelIDs* const& ModelIDs, const int& ActorCount, const int& PlayerIdx);
	virtual ~ActorManagerClass();

	inline IActorControlClass* const& GetPlayerInterface() const { return m_ActorInterfaces[m_PlayerIdx]; }
	inline const int& GetActorInterfaceCount() const { return m_ActorCount; }

	IActorControlClass* const& operator[](int idx) const;

private:
	HRESULT Initialize(const AffineInfo* const& ActorAffines, const AffineInfo* const& CollisionAffines, const ModelIDs* const& ModelIDs, const int& ActorCount, const int& PlayerIdx);
	void Shutdown();

private:
	static bool IsInitialize;

	IActorControlClass* m_ActorInterfaces[ACTOR_MAX_COUNT] = { nullptr, };
	int m_ActorCount = 0;
	int m_PlayerIdx = 0;

public:
	ActorManagerClass() = delete;
	ActorManagerClass(const ActorManagerClass& other) = delete;
};

