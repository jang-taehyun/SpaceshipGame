#pragma once

namespace Object
{
	class IMoveClass
	{
	public:
		virtual ~IMoveClass() = default;

		// 이동
		virtual DirectX::XMFLOAT4 Move(DirectX::XMFLOAT4 curPosition, DirectX::XMFLOAT4 rotate, MoveState state, float frame_time, bool IsKeyDown) = 0;

		// 움직임 속도 반환
		virtual float GetMoveSpeed() const = 0;

		// 움직임 속도 세팅
		virtual void SetMoveSpeed(float value) = 0;

		virtual std::unique_ptr<IMoveClass> Clone() const = 0;
	};
}