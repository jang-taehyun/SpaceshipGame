#pragma once

const int OtherMaxLimit = 6;

class ActorClass;

class ActorManagerClass
{
public:
	explicit ActorManagerClass();
	explicit ActorManagerClass(const AffineInfo& PlayerAffine, const ModelIDs PlayerModelID, const AffineInfo* const& OtherAffine, const int& OthersCount, const ModelIDs* const& OtherModelIDs);
	virtual ~ActorManagerClass();

	inline ActorClass* const& GetPlayerObject() const { return m_Player; }
	ActorClass* const& GetOtherObject(int idx) const;
	inline const int& GetOtherObjectCount() const { return m_OthersCount; }

private:
	HRESULT Initialize(const AffineInfo& PlayerAffine, const ModelIDs PlayerModelID, const AffineInfo* const& OtherAffine, const int& OthersCount, const ModelIDs* const& OtherModelIDs);
	void Shutdown();

private:
	static bool IsInitialize;

	ActorClass* m_Player = nullptr;
	ActorClass* m_Other[OtherMaxLimit] = { nullptr, };
	int m_OthersCount = 0;

public:
	ActorManagerClass(const ActorManagerClass& other) = delete;
};

