#pragma once

namespace Object
{
	class IRotateClass
	{
	public:
		virtual ~IRotateClass() = default;

		// 회전
		virtual DirectX::XMFLOAT4 Rotate(DirectX::XMFLOAT4 rotate, long MouseX, long MouseY, float frame_time, bool IsKeyDown) = 0;

		// 회전 속도 반환
		virtual float GetRoteteSpeed() const = 0;

		// 회전 속도 세팅
		virtual void SetRoteteSpeed(float value) = 0;

		virtual std::unique_ptr<IRotateClass> Clone() const = 0;

	private:

		// 회전 속도 계산
		virtual float ComputeRotateSpeed(float frame_time, bool IsKeyDown) = 0;
	};
}