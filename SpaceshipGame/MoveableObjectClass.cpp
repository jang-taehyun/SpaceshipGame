#include "pch.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "MoveableObjectClass.h"

MoveableObjectClass::MoveableObjectClass(IMoveClass* const& move, IRotateClass* const& rotate)
{
	m_Move = move;
	m_Rotate = rotate;
}

MoveableObjectClass::MoveableObjectClass(IMoveClass* const&& move, IRotateClass* const&& rotate)
{
	m_Move = move;
	m_Rotate = rotate;
}

MoveableObjectClass::~MoveableObjectClass()
{
	Shutdown();
}

void MoveableObjectClass::Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown)
{
	if (!m_Move)
		return;

	// 최종적으로 계산된 position으로 교체 //
	DirectX::XMFLOAT4 pos;
	
	switch (state)
	{
	case MoveState::MOVE_FORWARD:
	case MoveState::MOVE_BACKWARD:
		pos = m_Move->Move(GetPosition(), GetForwardVector(), state, frame_time, IsKeyDown);
		break;
	case MoveState::MOVE_LEFT:
	case MoveState::MOVE_RIGHT:
		pos = m_Move->Move(GetPosition(), GetRightVector(), state, frame_time, IsKeyDown);
		break;
	default:
		pos = m_Move->Move(GetPosition(), GetUpVector(), state, frame_time, IsKeyDown);
		break;
	}

	SetPosition(pos);
}

void MoveableObjectClass::Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	if (!m_Rotate)
		return;

	// 최종적으로 계산된 rotation으로 교체 //
	DirectX::XMFLOAT4 rot;

	m_Rotate->Rotate(GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);

	SetRotation(rot);
}

void MoveableObjectClass::Shutdown()
{
	if (m_Move)
	{
		delete m_Move;
		m_Move = nullptr;
	}

	if (m_Rotate)
	{
		delete m_Rotate;
		m_Rotate = nullptr;
	}
}
