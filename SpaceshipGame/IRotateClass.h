#pragma once

class IRotateClass
{
public:
	virtual ~IRotateClass() = default;

	// 회전
	virtual const DirectX::XMFLOAT4& Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown) = 0;

	// 회전 속도 반환
	virtual inline const float& GetRoteteSpeed() const = 0;

	// 회전 속도 세팅
	virtual inline void SetRoteteSpeed(const float& value) = 0;

private:

	// 회전 속도 계산
	virtual const float& ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown) = 0;
};

