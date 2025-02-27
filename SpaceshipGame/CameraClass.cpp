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

	// vector 설정 //
	
	// up vector 설정(카메라의 위쪽 방향 설정) //
	// vector에 값 설정
	Up.x = 0.f;
	Up.y = 1.f;
	Up.z = 0.f;

	// XMVECTOR 구조체에 저장
	UpVector = DirectX::XMLoadFloat3(&Up);

	// position vector 설정(3D 월드에서 카메라의 위치 설정) //
	// vector에 값 설정
	Position = m_Position;

	// XMVECTOR 구조체에 저장
	PositionVector = DirectX::XMLoadFloat3(&m_Position);

	// lookat vector 설정(카메라가 바라보고 있는 방향 설정) //
	// vector에 값 설정
	LookAt.x = 0.f;
	LookAt.y = 0.f;
	LookAt.z = 1.f;

	// XMVECTOR 구조체에 저장
	LookAtVector = DirectX::XMLoadFloat3(&LookAt);

	
	// 회전 행렬 생성 //
	// yaw, pitch, roll의 회전값을 라디안 단위로 설정 //
	pitch = DirectX::XMConvertToRadians(m_Rotation.x);
	yaw = DirectX::XMConvertToRadians(m_Rotation.y);
	roll = DirectX::XMConvertToRadians(m_Rotation.z);

	// pitch = m_Rotation.x * 0.0174532925f;
	// yaw = m_Rotation.y * 0.0174532925f;
	// roll = m_Rotation.z * 0.0174532925f;

	// 회전 행렬 생성
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);


	// lookat vector, up vector에 회전 행렬 적용 //
	// lookat vector, up vector에 회전 행렬를 적용하여,
	// lookat vector와 up vector를 월드 좌표계 기준으로 표현
	LookAtVector = DirectX::XMVector3TransformCoord(LookAtVector, RotationMatrix);
	UpVector = DirectX::XMVector3TransformCoord(UpVector, RotationMatrix);

	// 카메라가 바라보고 있는 방향을 월드 좌표계 기준으로 변환 //
	LookAtVector = DirectX::XMVectorAdd(PositionVector, LookAtVector);

	// lookat vector, up vector, position vector를 통해 view matrix 생성 //
	m_ViewMatrix = DirectX::XMMatrixLookAtLH(PositionVector, LookAtVector, UpVector);
}

void CameraClass::GetViewMatrix(DirectX::XMMATRIX& ViewMatrix)
{
	ViewMatrix = m_ViewMatrix;
}
