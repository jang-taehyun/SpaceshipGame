#pragma once

/**
* IActorControlClass 개요
* - ActorClass의 interface
* - Actor의 행동(Behavior)
*/

#include <DirectXCollision.h>

class ActorClass;

class IActorControlClass
{
public:
	IActorControlClass(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~IActorControlClass();

	void Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	void Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

	const DirectX::ContainmentType& GetCollideState(const AffineInfo& affine);
	const DirectX::ContainmentType& GetCollideState(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& forward, float& CollideDistance);

	inline const DirectX::XMFLOAT4X4& GetActorAffineMatrix() const;
	inline const DirectX::XMFLOAT4X4& GetCollisionAffineMatrix() const;

private:
	HRESULT Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	void Shutdown();

private:
	ActorClass* m_Actor = nullptr;

public:
	IActorControlClass() = delete;
	IActorControlClass(const IActorControlClass& other) = delete;
};

