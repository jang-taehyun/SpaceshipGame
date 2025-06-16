#include "pch.h"
#include "ComputeDirectionVectorClass.h"
#include "MoveClass.h"

MoveClass::MoveClass(float speed) : m_MoveSpeed(speed) {}

DirectX::XMFLOAT4 MoveClass::Move(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 rotate, MoveState state, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 ret;
	DirectX::XMFLOAT4 vector;
	float speed = 0.f;

	// speed 계산 //
	speed = ComputeMoveSpeed(frame_time, IsKeyDown);

	// 물체가 이동한 최종 position 구하고 반환 //
	switch (state)
	{
	case MoveState::MOVE_FORWARD:
		vector = ComputeDirectionVectorClass::GetForwardVector(rotate);
		ret = MoveForward(curPosition, vector, speed);
		break;
	case MoveState::MOVE_BACKWARD:
		vector = ComputeDirectionVectorClass::GetForwardVector(rotate);
		ret = MoveBackward(curPosition, vector, speed);
		break;
	case MoveState::MOVE_LEFT:
		vector = ComputeDirectionVectorClass::GetRightVector(rotate);
		ret = MoveLeft(curPosition, vector, speed);
		break;
	case MoveState::MOVE_RIGHT:
		vector = ComputeDirectionVectorClass::GetRightVector(rotate);
		ret = MoveRight(curPosition, vector, speed);
		break;
	default:
		ret = curPosition;
		break;
	}

	return ret;
}

std::unique_ptr<IMoveClass> MoveClass::Clone() const
{
	return std::make_unique<MoveClass>(*this);
}

DirectX::XMFLOAT4 MoveClass::MoveLeft(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 RightVector, float speed) const
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

DirectX::XMFLOAT4 MoveClass::MoveRight(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 RightVector, float speed) const
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

DirectX::XMFLOAT4 MoveClass::MoveForward(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 ForwardVector, float speed) const
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

DirectX::XMFLOAT4 MoveClass::MoveBackward(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 ForwardVector, float speed) const
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

float MoveClass::ComputeMoveSpeed(float frame_time, bool IsKeyDown)
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