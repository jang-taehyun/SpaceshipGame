#include "pch.h"
#include <DirectXMath.h>
#include "TransformClass.h"

static ErrorContent e;

TransformClass::TransformClass()
{
	DirectX::XMFLOAT4 position = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 rotation = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 scaling = { 1.f, 1.f, 1.f, 1.f };

	Initialize(position, rotation, scaling);
}

TransformClass::TransformClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling)
{
	Initialize(position, rotation, scaling);
}

TransformClass::~TransformClass() {}

void TransformClass::SetPosition(const DirectX::XMFLOAT4& pos)
{
	m_Position = pos;
	UpdateAffineMatrix();
}

void TransformClass::SetPosition(const float& x, const float& y, const float& z)
{
	m_Position = DirectX::XMFLOAT4(x, y, z, 1.f);
	UpdateAffineMatrix();
}

void TransformClass::SetRotation(const DirectX::XMFLOAT4& rot)
{ 
	m_Rotation = rot;
	UpdateAffineMatrix();
}

void TransformClass::SetRotation(const float& x, const float& y, const float& z)
{
	m_Rotation = DirectX::XMFLOAT4(x, y, z, 1.f);
	UpdateAffineMatrix();
}

void TransformClass::Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Scaling = scaling;

	UpdateAffineMatrix();
}

HRESULT TransformClass::ChangePosition(const MoveState& state, const bool& IsKeyDown)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TransformClass ChangePosition()");

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

void TransformClass::ChangeRotation(const long& MouseX, const long& MouseY)
{
	// yaw 업데이트 //
	m_Rotation.y += ((float)MouseX * m_MouseSensitivity);

	// pitch 업데이트 //
	m_Rotation.x += ((float)MouseY * m_MouseSensitivity);

	UpdateAffineMatrix();
}

void TransformClass::MoveLeft(const bool& IsKeyDown)
{
	if (IsKeyDown)
	{
		m_LeftMoveSpeed += (m_FrameTime * 0.01f);

		if (m_LeftMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_LeftMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_LeftMoveSpeed -= (m_FrameTime * 0.005f);

		if (m_LeftMoveSpeed < 0.f)
		{
			m_LeftMoveSpeed = 0.f;
		}
	}

	m_Position.x -= m_LeftMoveSpeed;
}

void TransformClass::MoveRight(const bool& IsKeyDown)
{
	if (IsKeyDown)
	{
		m_RightMoveSpeed += (m_FrameTime * 0.01f);

		if (m_RightMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_RightMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_RightMoveSpeed -= (m_FrameTime * 0.005f);

		if (m_RightMoveSpeed < 0.f)
		{
			m_RightMoveSpeed = 0.f;
		}
	}

	m_Position.x += m_RightMoveSpeed;
}

void TransformClass::MoveForward(const bool& IsKeyDown)
{
	if (IsKeyDown)
	{
		m_ForwardMoveSpeed += (m_FrameTime * 0.01f);

		if (m_ForwardMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_ForwardMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_ForwardMoveSpeed -= (m_FrameTime * 0.005f);

		if (m_ForwardMoveSpeed < 0.f)
		{
			m_ForwardMoveSpeed = 0.f;
		}
	}

	m_Position.z += m_ForwardMoveSpeed;
}

void TransformClass::MoveBackward(const bool& IsKeyDown)
{
	if (IsKeyDown)
	{
		m_BackwardMoveSpeed += (m_FrameTime * 0.01f);

		if (m_BackwardMoveSpeed > (m_FrameTime * m_KeyboardSensitivity))
		{
			m_BackwardMoveSpeed = m_FrameTime * m_KeyboardSensitivity;
		}
	}
	else
	{
		m_BackwardMoveSpeed -= (m_FrameTime * 0.005f);

		if (m_BackwardMoveSpeed < 0.f)
		{
			m_BackwardMoveSpeed = 0.f;
		}
	}

	m_Position.z -= m_BackwardMoveSpeed;
}

void TransformClass::UpdateAffineMatrix()
{
	DirectX::XMMATRIX scal = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat4(&m_Scaling));
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat4(&m_Rotation));
	DirectX::XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat4(&m_Position));

	m_Affine = scal * rot * trans;
}
