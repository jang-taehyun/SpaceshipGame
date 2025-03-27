#include "pch.h"
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
		break;
	case MoveState::MOVE_BACKWARD:
		break;
	case MoveState::MOVE_LEFT:
		break;
	case MoveState::MOVE_RIGHT:
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

HRESULT TransformClass::ChangeRotation(const RotationState& state, const bool& IsMouseMove)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TransformClass ChangeRotation()");

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
		e.contents = _T("예상치 못한 동작입니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
		break;
	}

	UpdateAffineMatrix();

	return result;
}

void TransformClass::TurnLeft(const bool& IsMouseMove)
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

void TransformClass::TurnRight(const bool& IsMouseMove)
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

void TransformClass::UpdateAffineMatrix()
{
	m_Affine = DirectX::XMMatrixAffineTransformation(
		DirectX::XMLoadFloat4(&m_Scaling),
		DirectX::XMLoadFloat4(&m_Position),
		DirectX::XMLoadFloat4(&m_Rotation),
		DirectX::XMLoadFloat4(&m_Position)
	);
}
