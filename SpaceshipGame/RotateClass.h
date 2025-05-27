#pragma once

/**
* RotateClass 개요
* - object(actor, collision 등)의 회전 관련된 Behavior 처리
*/

#include "IRotateClass.h"

class RotateClass : public IRotateClass
{
public:
	RotateClass() = default;
	explicit RotateClass(const float& speed);
	explicit RotateClass(const float&& speed);
	RotateClass(const RotateClass& other) = default;
	virtual ~RotateClass() = default;

	const DirectX::XMFLOAT4& Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown) override;

	inline const float& GetRoteteSpeed() const override { return m_RotateSpeed; }
	inline void SetRoteteSpeed(const float& value) override { m_RotateSpeed = value; }

private:
	const float& ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown) override;

private:
	float m_RotateSpeed = 1.f;
	float m_PrevRotateSpeed = 0.f;
};

