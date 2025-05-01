#pragma once

#include <SimpleMath.h>
#include "ActorClass.h"

class PlayerClass : public ActorClass
{
public:
	explicit PlayerClass(const AffineInfo& affine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~PlayerClass();

	inline const DirectX::SimpleMath::Ray* const& GetRay() const { return m_Ray; }

private:
	HRESULT IntializeRay(const AffineInfo& affine);
	void ShutdownRay();

private:
	DirectX::SimpleMath::Ray* m_Ray = nullptr;

public:
	PlayerClass(const PlayerClass& other) = delete;
};

