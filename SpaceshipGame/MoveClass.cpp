#include "pch.h"
#include "MoveClass.h"

MoveClass::MoveClass(const float& speed)
{
	m_MoveSpeed = speed;
}

MoveClass::MoveClass(const float&& speed)
{
	m_MoveSpeed = speed;
}

const DirectX::XMFLOAT4& MoveClass::Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown)
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


void MoveClass::GetDirectionVectors(DirectX::XMFLOAT4& forward, DirectX::XMFLOAT4& right, DirectX::XMFLOAT4& up) const
{
	float pitch, yaw, roll;
	DirectX::XMVECTOR f, r, u;
	DirectX::XMMATRIX RotationMatrix;

	// 해당 물체의 local space의 rotate matrix를 이용해, forward, right, up vector 계산 //
	// rotate matrix 생성
	pitch = m_Rotation.x;
	yaw = m_Rotation.y;
	roll = m_Rotation.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(forward, right, up vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	f = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	r = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	u = DirectX::XMVector3Normalize(RotationMatrix.r[1]);

	// XMFLAOT4 형식으로 저장
	DirectX::XMStoreFloat4(&forward, f);
	DirectX::XMStoreFloat4(&right, r);
	DirectX::XMStoreFloat4(&up, u);
}

const DirectX::XMFLOAT4& MoveClass::GetForwardVector() const
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR forward;
	DirectX::XMFLOAT4 ret;

	// 해당 물체의 local space의 rotate matrix를 이용해, forward vector 계산 //
	// rotate matrix 생성
	pitch = m_Rotation.x;
	yaw = m_Rotation.y;
	roll = m_Rotation.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(forward vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	forward = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	DirectX::XMStoreFloat4(&ret, forward);

	return ret;
}

const DirectX::XMFLOAT4& MoveClass::GetRightVector() const
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR right;
	DirectX::XMFLOAT4 ret;

	// 해당 물체의 local space의 rotate matrix를 이용해, right vector 계산 //
	// rotate matrix 생성
	pitch = m_Rotation.x;
	yaw = m_Rotation.y;
	roll = m_Rotation.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(right vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	right = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	DirectX::XMStoreFloat4(&ret, right);

	return ret;
}

const DirectX::XMFLOAT4& MoveClass::GetUpVector() const
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR up;
	DirectX::XMFLOAT4 ret;

	// 해당 물체의 local space의 rotate matrix를 이용해, up vector 계산 //
	// rotate matrix 생성
	pitch = m_Rotation.x;
	yaw = m_Rotation.y;
	roll = m_Rotation.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(up vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	up = DirectX::XMVector3Normalize(RotationMatrix.r[1]);
	DirectX::XMStoreFloat4(&ret, up);

	return ret;
}


DirectX::XMFLOAT4& MoveClass::MoveLeft(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed)
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

DirectX::XMFLOAT4& MoveClass::MoveRight(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed)
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

DirectX::XMFLOAT4& MoveClass::MoveForward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed)
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

DirectX::XMFLOAT4& MoveClass::MoveBackward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed)
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

const float& MoveClass::ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown)
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