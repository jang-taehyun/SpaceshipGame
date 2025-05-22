#pragma once

/**
* IObjectMoveableClass 개요
* - object(actor, collision 등)의 움직임 관련된 Behavior 처리
*/

#include "IMoveClass.h"

class MoveClass : public IMoveClass
{
public:
	MoveClass() = default;
	MoveClass(const MoveClass& other);
	MoveClass(const MoveClass&& other);
	virtual ~MoveClass() = default;

	const DirectX::XMFLOAT4& Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown) override;
	const DirectX::XMFLOAT4& Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown) override;

	inline const float& GetMoveSpeed() const override { return m_MoveSpeed; }
	inline const float& GetRoteteSpeed() const override { return m_RotateSpeed; }

	inline void SetMoveSpeed(const float& value) override { m_MoveSpeed = value; }
	inline void SetRoteteSpeed(const float& value) override { m_RotateSpeed = value; }

	virtual MoveClass& operator=(const MoveClass& other);
	virtual MoveClass& operator=(const MoveClass&& other);

private:
	DirectX::XMFLOAT4& MoveLeft(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed) override;
	DirectX::XMFLOAT4& MoveRight(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed) override;
	DirectX::XMFLOAT4& MoveForward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed) override;
	DirectX::XMFLOAT4& MoveBackward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed) override;

	const float& ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown) override;
	const float& ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown) override;

private:
	float m_RotateSpeed = 1.f;
	float m_PrevRotateSpeed = 0.f;
	float m_MoveSpeed = 1.f;
	float m_PrevMoveSpeed = 0.f;
};

