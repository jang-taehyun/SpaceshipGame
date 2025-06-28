#include "pch.h"
#include "ComputeDirectionVectorClass.h"

void Object::ComputeDirectionVectorClass::GetDirectionVectors(DirectX::XMFLOAT4 rotate, DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 right, DirectX::XMFLOAT4 up)
{
	float pitch, yaw, roll;
	DirectX::XMVECTOR f, r, u;
	DirectX::XMMATRIX RotationMatrix;

	// 해당 물체의 local space의 rotate matrix를 이용해, forward, right, up vector 계산 //
	// rotate matrix 생성
	pitch = rotate.x;
	yaw = rotate.y;
	roll = rotate.z;
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

DirectX::XMFLOAT4 Object::ComputeDirectionVectorClass::GetForwardVector(DirectX::XMFLOAT4 rotate)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR forward;
	DirectX::XMFLOAT4 ret;

	// 해당 물체의 local space의 rotate matrix를 이용해, forward vector 계산 //
	// rotate matrix 생성
	pitch = rotate.x;
	yaw = rotate.y;
	roll = rotate.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(forward vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	forward = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	DirectX::XMStoreFloat4(&ret, forward);

	return ret;
}

DirectX::XMFLOAT4 Object::ComputeDirectionVectorClass::GetRightVector(DirectX::XMFLOAT4 rotate)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR right;
	DirectX::XMFLOAT4 ret;

	// 해당 물체의 local space의 rotate matrix를 이용해, right vector 계산 //
	// rotate matrix 생성
	pitch = rotate.x;
	yaw = rotate.y;
	roll = rotate.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(right vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	right = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	DirectX::XMStoreFloat4(&ret, right);

	return ret;
}

DirectX::XMFLOAT4 Object::ComputeDirectionVectorClass::GetUpVector(DirectX::XMFLOAT4 rotate)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;
	DirectX::XMVECTOR up;
	DirectX::XMFLOAT4 ret;

	// 해당 물체의 local space의 rotate matrix를 이용해, up vector 계산 //
	// rotate matrix 생성
	pitch = rotate.x;
	yaw = rotate.y;
	roll = rotate.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(up vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	up = DirectX::XMVector3Normalize(RotationMatrix.r[1]);
	DirectX::XMStoreFloat4(&ret, up);

	return ret;
}