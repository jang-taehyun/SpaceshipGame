#include "pch.h"
#include "CameraClass.h"

CameraClass::CameraClass() {}
CameraClass::CameraClass(const CameraClass& other) {}
CameraClass::~CameraClass() {}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

DirectX::XMFLOAT3 CameraClass::GetPosition()
{
	return m_Position;
}

DirectX::XMFLOAT3 CameraClass::GetRotation()
{
	return m_Rotation;
}

void CameraClass::Render()
{
	DirectX::XMFLOAT3 Up, Position, LookAt;
	DirectX::XMVECTOR UpVector, PositionVector, LookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX RotationMatrix;

	// vector ���� //
	
	// up vector ����(ī�޶��� ���� ���� ����) //
	// vector�� �� ����
	Up.x = 0.f;
	Up.y = 1.f;
	Up.z = 0.f;

	// XMVECTOR ����ü�� ����
	UpVector = DirectX::XMLoadFloat3(&Up);

	// position vector ����(3D ���忡�� ī�޶��� ��ġ ����) //
	// vector�� �� ����
	Position = m_Position;

	// XMVECTOR ����ü�� ����
	PositionVector = DirectX::XMLoadFloat3(&m_Position);

	// lookat vector ����(ī�޶� �ٶ󺸰� �ִ� ���� ����) //
	// vector�� �� ����
	LookAt.x = 0.f;
	LookAt.y = 0.f;
	LookAt.z = 1.f;

	// XMVECTOR ����ü�� ����
	LookAtVector = DirectX::XMLoadFloat3(&LookAt);

	
	// ȸ�� ��� ���� //
	// yaw, pitch, roll�� ȸ������ ���� ������ ���� //
	pitch = DirectX::XMConvertToRadians(m_Rotation.x);
	yaw = DirectX::XMConvertToRadians(m_Rotation.y);
	roll = DirectX::XMConvertToRadians(m_Rotation.z);

	// pitch = m_Rotation.x * 0.0174532925f;
	// yaw = m_Rotation.y * 0.0174532925f;
	// roll = m_Rotation.z * 0.0174532925f;

	// ȸ�� ��� ����
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);


	// lookat vector, up vector�� ȸ�� ��� ���� //
	// lookat vector, up vector�� ȸ�� ��ĸ� �����Ͽ�,
	// lookat vector�� up vector�� ���� ��ǥ�� �������� ǥ��
	LookAtVector = DirectX::XMVector3TransformCoord(LookAtVector, RotationMatrix);
	UpVector = DirectX::XMVector3TransformCoord(UpVector, RotationMatrix);

	// ī�޶� �ٶ󺸰� �ִ� ������ ���� ��ǥ�� �������� ��ȯ //
	LookAtVector = DirectX::XMVectorAdd(PositionVector, LookAtVector);

	// lookat vector, up vector, position vector�� ���� view matrix ���� //
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);
}

void CameraClass::GetViewMatrix(DirectX::XMMATRIX& ViewMatrix)
{
	ViewMatrix = m_ViewMatrix;
}
