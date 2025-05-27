#pragma once

/**
* MoveClass 개요
* - object(actor, collision 등)의 움직임 관련된 Behavior 처리
*/

#include "IMoveClass.h"

class MoveClass : public IMoveClass
{
public:
	MoveClass() = default;
	explicit MoveClass(const float& speed);
	explicit MoveClass(const float&& speed);
	MoveClass(const MoveClass& other) = default;
	virtual ~MoveClass() = default;

	const DirectX::XMFLOAT4& Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown) override;
	
	inline const float& GetMoveSpeed() const override { return m_MoveSpeed; }
	inline void SetMoveSpeed(const float& value) override { m_MoveSpeed = value; }

private:
	virtual void GetDirectionVectors(DirectX::XMFLOAT4& forward, DirectX::XMFLOAT4& right, DirectX::XMFLOAT4& up) const override;
	virtual const DirectX::XMFLOAT4& GetForwardVector() const override;
	virtual const DirectX::XMFLOAT4& GetRightVector() const override;
	virtual const DirectX::XMFLOAT4& GetUpVector() const override;

	DirectX::XMFLOAT4& MoveLeft(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed) override;
	DirectX::XMFLOAT4& MoveRight(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed) override;
	DirectX::XMFLOAT4& MoveForward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed) override;
	DirectX::XMFLOAT4& MoveBackward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed) override;

	const float& ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown) override;

private:
	float m_MoveSpeed = 1.f;
	float m_PrevMoveSpeed = 0.f;
};

