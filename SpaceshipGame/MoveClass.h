#pragma once

/**
* MoveClass 개요
* - object(actor, collision 등)의 움직임 관련된 Behavior 처리
*/

#include "IMoveClass.h"

namespace Object
{
	class MoveClass : public IMoveClass
	{
	public:
		MoveClass() = default;
		explicit MoveClass(float speed);
		virtual ~MoveClass() = default;

		DirectX::XMFLOAT4 Move(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 rotate, MoveState state, float frame_time, bool IsKeyDown) override;

		inline float GetMoveSpeed() const override { return m_MoveSpeed; }
		inline void SetMoveSpeed(float value) override { m_MoveSpeed = value; }

		virtual std::unique_ptr<IMoveClass> Clone() const override;

	private:
		DirectX::XMFLOAT4 MoveLeft(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 RightVector, float speed) const;
		DirectX::XMFLOAT4 MoveRight(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 RightVector, float speed) const;
		DirectX::XMFLOAT4 MoveForward(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 ForwardVector, float speed) const;
		DirectX::XMFLOAT4 MoveBackward(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 ForwardVector, float speed) const;

		float ComputeMoveSpeed(float frame_time, bool IsKeyDown);

	private:
		float m_MoveSpeed = 1.f;
		float m_PrevMoveSpeed = 0.f;
	};
}