#pragma once

/**
* RotateClass 개요
* - object(actor, collision 등)의 회전 관련된 Behavior 처리
*/

#include "IRotateClass.h"

namespace Object
{
	class RotateClass : public IRotateClass
	{
	public:
		explicit RotateClass(float speed = 1.f);
		virtual ~RotateClass() = default;

		virtual DirectX::XMFLOAT4 Rotate(DirectX::XMFLOAT4 rotate, long MouseDeltaX, long MouseDeltaY, float frame_time) override;

		virtual float GetRoteteSpeed() const override { return m_RotateSpeed; }
		virtual void SetRoteteSpeed(float value) override { m_RotateSpeed = value; }

		virtual std::unique_ptr<IRotateClass> Clone() const override;

	private:
		float m_RotateSpeed = 1.f;
	};
}