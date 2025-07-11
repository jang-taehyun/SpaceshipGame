#include "pch.h"
#include "RotateClass.h"

Object::RotateClass::RotateClass(float speed) : m_RotateSpeed(speed) {}

DirectX::XMFLOAT4 Object::RotateClass::Rotate(DirectX::XMFLOAT4 rotate, long MouseX, long MouseY, float frame_time, bool IsKeyDown)
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

	if (ret.x > 89.f)
		ret.x = 89.f;
	if (ret.x < -89.f)
		ret.x = -89.f;

	// 변경된 rotate 반환 //
	return ret;
}

std::unique_ptr<Object::IRotateClass> Object::RotateClass::Clone() const
{
	return std::make_unique<RotateClass>(*this);
}

float Object::RotateClass::ComputeRotateSpeed(float frame_time, bool IsKeyDown)
{
	float ret = m_PrevRotateSpeed;

	if (IsKeyDown)
	{
		ret += (frame_time * System::MOUSE_SENSITIVITY * m_RotateSpeed);

		if (ret > (frame_time * System::MOUSE_SENSITIVITY * m_RotateSpeed))
		{
			ret = frame_time * System::MOUSE_SENSITIVITY * m_RotateSpeed;
		}
	}
	else
	{
		ret -= (frame_time * System::MOUSE_SENSITIVITY * m_RotateSpeed);

		if (ret < 0.f)
		{
			ret = 0.f;
		}
	}

	m_PrevRotateSpeed = ret;

	return ret;
}