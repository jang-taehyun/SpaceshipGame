#pragma once

/**
* IObjectMoveableClass 개요
* - object(actor, collision 등)의 움직임 관련된 Behavior 처리
*/

class IObjectMoveableClass
{
public:
	IObjectMoveableClass() = default;
	virtual ~IObjectMoveableClass() = default;

	const DirectX::XMFLOAT4& Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	const DirectX::XMFLOAT4& Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

	inline const float& GetMoveSpeed() const { return m_MoveSpeed; }
	inline const float& GetRoteteSpeed() const { return m_RotateSpeed; }

	inline void SetMoveSpeed(const float& value) { m_MoveSpeed = value; }
	inline void SetRoteteSpeed(const float& value) { m_RotateSpeed = value; }

private:
	DirectX::XMFLOAT4& MoveLeft(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed);
	DirectX::XMFLOAT4& MoveRight(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed);
	DirectX::XMFLOAT4& MoveForward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed);
	DirectX::XMFLOAT4& MoveBackward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed);

	const float& ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown);
	const float& ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown);

private:
	float m_RotateSpeed = 1.f;
	float m_PrevRotateSpeed = 0.f;
	float m_MoveSpeed = 1.f;
	float m_PrevMoveSpeed = 0.f;

public:
	IObjectMoveableClass(const IObjectMoveableClass& other) = delete;
};

