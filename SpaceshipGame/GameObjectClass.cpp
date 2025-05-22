#include "pch.h"
#include "TypeConverterClass.h"
#include "GameObjectClass.h"

const DirectX::XMFLOAT4X4& GameObjectClass::GetAffineMatrix() const
{
	DirectX::XMMATRIX affine;
	DirectX::XMFLOAT4X4 ret;

	// affine matrix = scale * rotate * pos
	affine = TypeConverterClass::XMFLOAT4toXMMATRIX(m_Position, m_Rotation, m_Scale);

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 후 반환 //
	DirectX::XMStoreFloat4x4(&ret, affine);
	return ret;
}

void GameObjectClass::GetDirectionVectors(DirectX::XMFLOAT4& forward, DirectX::XMFLOAT4& right, DirectX::XMFLOAT4& up) const
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

const DirectX::XMFLOAT4& GameObjectClass::GetForwardVector() const
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

const DirectX::XMFLOAT4& GameObjectClass::GetRightVector() const
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

const DirectX::XMFLOAT4& GameObjectClass::GetUpVector() const
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
