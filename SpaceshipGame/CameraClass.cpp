#include "pch.h"
#include "CameraClass.h"

CameraClass::CameraClass() {}
CameraClass::CameraClass(const CameraClass& other) {}
CameraClass::~CameraClass() {}

HRESULT CameraClass::Initialize()
{
	DirectX::XMFLOAT4 position = { 10.f, 1.f, -0.5f, 1.f };
	DirectX::XMFLOAT4 rotation = { 0.f, -90.f, 0.f, 1.f };
	DirectX::XMFLOAT4 scaling = { 1.f, 1.f, 1.f, 1.f };

	m_Position = new PositionClass;
	if (!m_Position)
	{
		return E_FAIL;
	}
	m_Position->Initialize(position, rotation, scaling);

	return S_OK;
}

void CameraClass::Render()
{
	DirectX::XMFLOAT4 Up, Position, LookAt;
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
	UpVector = DirectX::XMLoadFloat4(&Up);

	// position vector 설정(3D 월드에서 카메라의 위치 설정) //
	// vector에 값 설정
	Position = m_Position->GetPosition();

	// XMVECTOR 구조체에 저장
	PositionVector = DirectX::XMLoadFloat4(&Position);

	// lookat vector 설정(카메라가 바라보고 있는 방향 설정) //
	// vector에 값 설정
	LookAt.x = 0.f;
	LookAt.y = 0.f;
	LookAt.z = 1.f;

	// XMVECTOR 구조체에 저장
	LookAtVector = DirectX::XMLoadFloat4(&LookAt);

	
	// 회전 행렬 생성 //
	// yaw, pitch, roll의 회전값을 라디안 단위로 설정 //
	pitch = DirectX::XMConvertToRadians(m_Position->GetRotation().x);
	yaw = DirectX::XMConvertToRadians(m_Position->GetRotation().y);
	roll = DirectX::XMConvertToRadians(m_Position->GetRotation().z);

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
