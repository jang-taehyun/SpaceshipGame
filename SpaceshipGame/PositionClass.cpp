#include "pch.h"
#include "PositionClass.h"

PositionClass::PositionClass() {}
PositionClass::PositionClass(const PositionClass& other) {}
PositionClass::~PositionClass() {}

void PositionClass::Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Scaling = scaling;

	UpdateAffineMatrix();
}

HRESULT PositionClass::ChangePosition(const MoveState& state, const bool& IsKeyDown)
{
	switch (state)
	{
	case MoveState::MOVE_FORWARD:
		break;
	case MoveState::MOVE_BACKWARD:
		break;
	case MoveState::MOVE_LEFT:
		break;
	case MoveState::MOVE_RIGHT:
		break;
	default:
		return E_FAIL;
		break;
	}

	UpdateAffineMatrix();

	return S_OK;
}

HRESULT PositionClass::ChangeRotation(const RotationState& state, const bool& IsMouseMove)
{
	switch (state)
	{
	case RotationState::ROTATE_UP:
		break;
	case RotationState::ROTATE_DOWN:
		break;
	case RotationState::ROTATE_LEFT:
		TurnLeft(IsMouseMove);
		break;
	case RotationState::ROTATE_RIGHT:
		TurnRight(IsMouseMove);
		break;
	default:
		return E_FAIL;
		break;
	}

	UpdateAffineMatrix();

	return S_OK;
}

void PositionClass::TurnLeft(const bool& IsMouseMove)
{
	if (IsMouseMove)
	{
		m_LeftTurnSpeed += (m_FrameTime * 0.01f);

		if (m_LeftTurnSpeed > (m_FrameTime * 0.15f))
		{
			m_LeftTurnSpeed = m_FrameTime * 0.15f;
		}
	}
	else
	{
		m_LeftTurnSpeed -= (m_FrameTime * 0.005f);

		if (m_LeftTurnSpeed < 0.f)
		{
			m_LeftTurnSpeed = 0.f;
		}
	}

	m_Rotation.y -= m_LeftTurnSpeed;
	if (m_Rotation.y < 0.f)
	{
		m_Rotation.y += 360.f;
	}
}

void PositionClass::TurnRight(const bool& IsMouseMove)
{
	if (IsMouseMove)
	{
		m_RightTurnSpeed += (m_FrameTime * 0.01f);

		if (m_RightTurnSpeed > (m_FrameTime * 0.15f))
		{
			m_RightTurnSpeed = m_FrameTime * 0.15f;
		}
	}
	else
	{
		m_RightTurnSpeed -= (m_FrameTime * 0.005f);

		if (m_RightTurnSpeed < 0.f)
		{
			m_RightTurnSpeed = 0.f;
		}
	}

	m_Rotation.y += m_RightTurnSpeed;
	if (m_Rotation.y > 0.f)
	{
		m_Rotation.y -= 360.f;
	}
}

void PositionClass::UpdateAffineMatrix()
{
	m_Affine = DirectX::XMMatrixAffineTransformation(
		DirectX::XMLoadFloat4(&m_Scaling),
		DirectX::XMLoadFloat4(&m_Position),
		DirectX::XMLoadFloat4(&m_Rotation),
		DirectX::XMLoadFloat4(&m_Position)
	);
}
