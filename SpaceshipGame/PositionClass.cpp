#include "pch.h"
#include "PositionClass.h"

PositionClass::PositionClass() {}
PositionClass::PositionClass(const PositionClass& other) {}
PositionClass::~PositionClass() {}

void PositionClass::SetFrameTime(float time)
{
	m_FrameTime = time;
}

void PositionClass::GetRotation(float& y)
{
	y = m_RotationY;
}

void PositionClass::TurnLeft(bool IsKeyDown)
{
	if (IsKeyDown)
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

	m_RotationY -= m_LeftTurnSpeed;
	if (m_RotationY < 0.f)
	{
		m_RotationY += 360.f;
	}
}

void PositionClass::TurnRight(bool IsKeyDown)
{
	if (IsKeyDown)
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

	m_RotationY += m_RightTurnSpeed;
	if (m_RotationY > 0.f)
	{
		m_RotationY -= 360.f;
	}
}
