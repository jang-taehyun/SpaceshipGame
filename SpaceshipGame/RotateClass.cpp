#include "pch.h"
#include "RotateClass.h"

RotateClass::RotateClass(const float& speed)
{
	m_RotateSpeed = speed;
}

RotateClass::RotateClass(const float&& speed)
{
	m_RotateSpeed = speed;
}

const DirectX::XMFLOAT4& RotateClass::Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown)
{
	DirectX::XMFLOAT4 ret = rotate;
	float speed = 0.f;

	// speed 계산 //
	speed = ComputeRotateSpeed(frame_time, IsKeyDown);

	// yaw, pitch 업데이트 //
	// yaw
	ret.y += ((float)MouseX * speed);
	// pitch
	ret.x += ((float)MouseY * speed);

	// 변경된 rotate 반환 //
	return ret;
}

const float& RotateClass::ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown)
{
	float ret = m_PrevRotateSpeed;

	if (IsKeyDown)
	{
		ret += (frame_time * MOUSE_SENSITIVITY * m_RotateSpeed);

		if (ret > (frame_time * MOUSE_SENSITIVITY * m_RotateSpeed))
		{
			ret = frame_time * MOUSE_SENSITIVITY * m_RotateSpeed;
		}
	}
	else
	{
		ret -= (frame_time * MOUSE_SENSITIVITY * m_RotateSpeed);

		if (ret < 0.f)
		{
			ret = 0.f;
		}
	}

	m_PrevRotateSpeed = ret;

	return ret;
}