#include "pch.h"
#include "TypeConverter.h"
#include "AffineClass.h"

AffineClass::AffineClass(const AffineInfo& affine)
{
	Initialize(affine);
}

void AffineClass::GetDirectionVectors(DirectX::XMVECTOR& forward, DirectX::XMVECTOR& right, DirectX::XMVECTOR& up)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;

	// 해당 물체의 local space의 rotate matrix를 이용해, forward, right, up vector 계산 //
	// rotate matrix 생성
	pitch = m_Rotation.x;
	yaw = m_Rotation.y;
	roll = m_Rotation.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축(forward, right, up vector) 추출
	// 추출 시에 vector 정규화를 하고 추출
	forward = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	right = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	up = DirectX::XMVector3Normalize(RotationMatrix.r[1]);
}

const DirectX::XMFLOAT4& AffineClass::GetForwardVector() const
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

const DirectX::XMFLOAT4& AffineClass::GetRightVector() const
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

const DirectX::XMFLOAT4& AffineClass::GetUpVector() const
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

const DirectX::XMFLOAT4X4& AffineClass::GetAffine() const
{
	DirectX::XMMATRIX affine;
	DirectX::XMFLOAT4X4 ret;

	// affine matrix = scale * rotate * pos
	affine = TypeConverter::XMFLOAT4toXMMATRIX(m_Position, m_Rotation, m_Scaling);

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 후 반환 //
	DirectX::XMStoreFloat4x4(&ret, affine);
	return ret;
}

void AffineClass::Initialize(const AffineInfo& affine)
{
	m_Position = affine.position;
	m_Rotation = affine.rotation;
	m_Scaling = affine.scale;
}
