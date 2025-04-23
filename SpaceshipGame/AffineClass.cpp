#include "pch.h"
#include <DirectXMath.h>
#include "AffineClass.h"

static ErrorContent e;

AffineClass::AffineClass()
{
	AffineInfo affine;
	affine.position = { 0.f, 0.f, 0.f, 1.f };
	affine.rotation = { 0.f, 0.f, 0.f, 1.f };
	affine.scale = { 1.f, 1.f, 1.f, 1.f };

	Initialize(affine);
}

AffineClass::AffineClass(const AffineInfo& affine)
{
	Initialize(affine);
}

AffineClass::~AffineClass() {}

void AffineClass::SetPosition(const DirectX::XMFLOAT4& pos)
{
	m_Position = pos;
	UpdateAffineMatrix();
}

void AffineClass::SetPosition(const float& x, const float& y, const float& z)
{
	m_Position = DirectX::XMFLOAT4(x, y, z, 1.f);
	UpdateAffineMatrix();
}

void AffineClass::SetRotation(const DirectX::XMFLOAT4& rot)
{ 
	m_Rotation = rot;
	UpdateAffineMatrix();
}

void AffineClass::SetRotation(const float& x, const float& y, const float& z)
{
	m_Rotation = DirectX::XMFLOAT4(x, y, z, 1.f);
	UpdateAffineMatrix();
}

void AffineClass::SetScale(const DirectX::XMFLOAT4& scale)
{
	m_Scaling = scale;
	UpdateAffineMatrix();
}

void AffineClass::SetScale(const float& x, const float& y, const float& z)
{
	m_Scaling = DirectX::XMFLOAT4(x, y, z, 1.f);
	UpdateAffineMatrix();
}

void AffineClass::Initialize(const AffineInfo& affine)
{
	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;

	m_Position = affine.position;
	m_Rotation = affine.rotation;
	m_Scaling = affine.scale;

	// forward, right, up vector 설정 //
	// 회전 행렬 설정 //
	pitch = m_Position.x;
	yaw = m_Position.y;
	roll = m_Position.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 카메라의 local 좌표계의 축 추출 //
	m_ForwardVector = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	m_RightVector = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	m_UpVector = DirectX::XMVector3Normalize(RotationMatrix.r[1]);

	UpdateAffineMatrix();
}

HRESULT AffineClass::ChangePosition(const MoveState& state, const bool& IsKeyDown)
{
	HRESULT result = S_OK;

	float pitch, yaw, roll;
	DirectX::XMMATRIX RotationMatrix;

	// 에러 메세지 초기화 //
	e.title = _T("TransformClass ChangePosition()");

	// 회전 행렬 설정 //
	pitch = m_Rotation.x;
	yaw = m_Rotation.y;
	roll = m_Rotation.z;
	RotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 해당 객체의 local 좌표계의 축 추출 //
	m_ForwardVector = DirectX::XMVector3Normalize(RotationMatrix.r[2]);
	m_RightVector = DirectX::XMVector3Normalize(RotationMatrix.r[0]);
	m_UpVector = DirectX::XMVector3Normalize(RotationMatrix.r[1]);

	switch (state)
	{
	case MoveState::MOVE_FORWARD:
		MoveForward(IsKeyDown);
		break;
	case MoveState::MOVE_BACKWARD:
		MoveBackward(IsKeyDown);
		break;
	case MoveState::MOVE_LEFT:
		MoveLeft(IsKeyDown);
		break;
	case MoveState::MOVE_RIGHT:
		MoveRight(IsKeyDown);
		break;
	default:
		e.contents = _T("예상치 못한 동작입니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
		break;
	}

	UpdateAffineMatrix();

	return result;
}

void AffineClass::ChangeRotation(const long& MouseX, const long& MouseY)
{
	// yaw 업데이트 //
	m_Rotation.y += ((float)MouseX * m_MouseSensitivity);

	// pitch 업데이트 //
	m_Rotation.x += ((float)MouseY * m_MouseSensitivity);

	UpdateAffineMatrix();
}

void AffineClass::MoveLeft(const bool& IsKeyDown)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&m_Position);

	if (IsKeyDown)
	{
		m_LeftMoveSpeed += (m_FrameTime * m_KeyboardSensitivity);

		if (m_LeftMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_LeftMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_LeftMoveSpeed -= (m_FrameTime * m_KeyboardSensitivity);

		if (m_LeftMoveSpeed < 0.f)
		{
			m_LeftMoveSpeed = 0.f;
		}
	}

	SetMoveSpeed(m_LeftMoveSpeed);

	position -= m_RightVector * m_LeftMoveSpeed;
	XMStoreFloat4(&m_Position, position);
}

void AffineClass::MoveRight(const bool& IsKeyDown)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&m_Position);

	if (IsKeyDown)
	{
		m_RightMoveSpeed += (m_FrameTime * m_KeyboardSensitivity);

		if (m_RightMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_RightMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_RightMoveSpeed -= (m_FrameTime * m_KeyboardSensitivity);

		if (m_RightMoveSpeed < 0.f)
		{
			m_RightMoveSpeed = 0.f;
		}
	}

	SetMoveSpeed(m_RightMoveSpeed);

	position += m_RightVector * m_RightMoveSpeed;
	XMStoreFloat4(&m_Position, position);
}

void AffineClass::MoveForward(const bool& IsKeyDown)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&m_Position);

	if (IsKeyDown)
	{
		m_ForwardMoveSpeed += (m_FrameTime * m_KeyboardSensitivity);

		if (m_ForwardMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_ForwardMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_ForwardMoveSpeed -= (m_FrameTime * m_KeyboardSensitivity);

		if (m_ForwardMoveSpeed < 0.f)
		{
			m_ForwardMoveSpeed = 0.f;
		}
	}

	SetMoveSpeed(m_ForwardMoveSpeed);

	position += m_ForwardVector * m_ForwardMoveSpeed;
	XMStoreFloat4(&m_Position, position);
}

void AffineClass::MoveBackward(const bool& IsKeyDown)
{
	using namespace DirectX;

	XMVECTOR position = XMLoadFloat4(&m_Position);

	if (IsKeyDown)
	{
		m_BackwardMoveSpeed += (m_FrameTime * m_KeyboardSensitivity);

		if (m_BackwardMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_BackwardMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_BackwardMoveSpeed -= (m_FrameTime * m_KeyboardSensitivity);

		if (m_BackwardMoveSpeed < 0.f)
		{
			m_BackwardMoveSpeed = 0.f;
		}
	}

	SetMoveSpeed(m_BackwardMoveSpeed);

	position -= m_ForwardVector * m_BackwardMoveSpeed;
	XMStoreFloat4(&m_Position, position);
}

void AffineClass::UpdateAffineMatrix()
{
	DirectX::XMMATRIX scal = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat4(&m_Scaling));
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat4(&m_Rotation));
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&m_Position));

	m_Affine = scal * rot * trans;
}
