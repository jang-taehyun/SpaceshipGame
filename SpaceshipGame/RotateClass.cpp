#include "pch.h"
#include "RotateClass.h"

Object::RotateClass::RotateClass(float speed)
	: m_RotateSpeed(speed)
{}

DirectX::XMFLOAT4 Object::RotateClass::Rotate(DirectX::XMFLOAT4 rotate, long MouseDeltaX, long MouseDeltaY, float frame_time)
{
	DirectX::XMFLOAT4 ret = rotate;
	float speed = 0.f;

	// speed 계산 //
	speed = frame_time * System::INTERPOLATION_MOUSE_SPEED * m_RotateSpeed;

	// yaw, pitch 업데이트 //
	// yaw
	ret.y += ((float)MouseDeltaX * speed);
	// pitch
	ret.x += ((float)MouseDeltaY * speed);

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