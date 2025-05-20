#pragma once

/**
* IActorControlClass 개요
* - ActorClass의 interface
* - Affine 관련 연산 실행
*	- forward, backward, left, right 이동 실행
*	- rotate 실행
*	- affine matrix 리턴
* - 충돌 검사 실행
*/

#include <SimpleMath.h>
#include <DirectXCollision.h>

class ActorClass;

class IActorControlClass
{
public:
	explicit IActorControlClass(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	virtual ~IActorControlClass();

	void Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	void Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

	const DirectX::ContainmentType& GetCollideState(const DirectX::BoundingOrientedBox* const& collision);
	const DirectX::ContainmentType& GetCollideState(const DirectX::SimpleMath::Ray* const& ray);

	inline const DirectX::BoundingOrientedBox* const& GetCollision() const;
	inline const DirectX::XMFLOAT4X4& GetActorAffineMatrix() const;
	inline const DirectX::XMFLOAT4X4& GetCollisionAffineMatrix() const;

private:
	HRESULT Initailize(const AffineInfo& ModelAffine, const AffineInfo& CollisionAffine, const ModelIDs ModelID = ModelIDs::DEFAULT_SPACESHIP);
	void Shutdown();

private:
	ActorClass* m_Actor;

public:
	IActorControlClass() = delete;
	IActorControlClass(const IActorControlClass& other) = delete;
};

