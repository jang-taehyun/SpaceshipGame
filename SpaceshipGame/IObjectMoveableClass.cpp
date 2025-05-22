#include "pch.h"
#include "IObjectMoveableClass.h"

const DirectX::XMFLOAT4& IObjectMoveableClass::Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 ret;
	float speed = 0.f;

	// speed 계산 //
	speed = ComputeMoveSpeed(frame_time, IsKeyDown);

	// 물체가 이동한 최종 position 구하고, 최종 position 반환 //
	switch (state)
	{
	case MoveState::MOVE_FORWARD:
		ret = MoveForward(curPosition, vector, speed);
		break;
	case MoveState::MOVE_BACKWARD:
		ret = MoveBackward(curPosition, vector, speed);
		break;
	case MoveState::MOVE_LEFT:
		ret = MoveLeft(curPosition, vector, speed);
		break;
	case MoveState::MOVE_RIGHT:
		ret = MoveRight(curPosition, vector, speed);
		break;
	default:
		ret = curPosition;
		break;
	}

	return ret;
}

const DirectX::XMFLOAT4& IObjectMoveableClass::Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 ret = rotate;
	float speed = 0.f;

	// speed 계산 //
	speed = ComputeRotateSpeed(frame_time, IsKeyDown);

	// yaw, pitch 업데이트 //
	// yaw
	ret.y += ((float)MouseX * speed);
	// pitch
	ret.x += ((float)MouseY * speed);

	// 변경된 rotate 반환 //
	return ret;
}


DirectX::XMFLOAT4& IObjectMoveableClass::MoveLeft(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed)
{
	using namespace DirectX;

	XMVECTOR vector = XMLoadFloat4(&RightVector);
	XMVECTOR position = XMLoadFloat4(&curPosition);
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position -= (vector * speed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& IObjectMoveableClass::MoveRight(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed)
{
	using namespace DirectX;

	XMVECTOR vector = XMLoadFloat4(&RightVector);
	XMVECTOR position = XMLoadFloat4(&(curPosition));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position += (vector * speed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& IObjectMoveableClass::MoveForward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed)
{
	using namespace DirectX;

	XMVECTOR vector = XMLoadFloat4(&ForwardVector);
	XMVECTOR position = XMLoadFloat4(&(curPosition));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position += (vector * speed);
	XMStoreFloat4(&ret, position);

	return ret;
}

DirectX::XMFLOAT4& IObjectMoveableClass::MoveBackward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed)
{
	using namespace DirectX;

	XMVECTOR vector = XMLoadFloat4(&ForwardVector);
	XMVECTOR position = XMLoadFloat4(&(curPosition));
	XMFLOAT4 ret;

	// 최종 position 계산 및 반환 //
	position -= (vector * speed);
	XMStoreFloat4(&ret, position);

	return ret;
}

const float& IObjectMoveableClass::ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown)
{
	float ret = m_PrevMoveSpeed;

	if (IsKeyDown)
	{
		ret += (frame_time * m_MoveSpeed);

		if (ret > (frame_time * m_MoveSpeed))
		{
			ret = frame_time * m_MoveSpeed;
		}
	}
	else
	{
		ret -= (frame_time * m_MoveSpeed);

		if (ret < 0.f)
		{
			ret = 0.f;
		}
	}

	m_PrevMoveSpeed = ret;

	return ret;
}

const float& IObjectMoveableClass::ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown)
{
	float ret = m_PrevRotateSpeed;

	if (IsKeyDown)
	{
		ret += (frame_time * MOUSE_SENSITIVITY * m_RotateSpeed);

		if (ret > (frame_time * MOUSE_SENSITIVITY * m_RotateSpeed))
		{
			ret = frame_time * MOUSE_SENSITIVITY * m_RotateSpeed;
		}
	}
	else
	{
		ret -= (frame_time * MOUSE_SENSITIVITY * m_RotateSpeed);

		if (ret < 0.f)
		{
			ret = 0.f;
		}
	}

	m_PrevRotateSpeed = ret;

	return ret;
}