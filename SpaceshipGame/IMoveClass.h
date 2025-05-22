#pragma once

class IMoveClass
{
public:
	virtual ~IMoveClass() = default;

	// 이동
	virtual const DirectX::XMFLOAT4& Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown) = 0;
	
	// 회전
	virtual const DirectX::XMFLOAT4& Rotate(const DirectX::XMFLOAT4& rotate, const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown) = 0;
	 
	// 움직임 속도 반환
	virtual inline const float& GetMoveSpeed() const = 0;

	// 회전 속도 반환
	virtual inline const float& GetRoteteSpeed() const = 0;
	
	// 움직임 속도 세팅
	virtual inline void SetMoveSpeed(const float& value) = 0;

	// 회전 속도 세팅
	virtual inline void SetRoteteSpeed(const float& value) = 0;

private:

	// 왼쪽 이동
	virtual DirectX::XMFLOAT4& MoveLeft(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed) = 0;

	// 오른쪽 이동
	virtual DirectX::XMFLOAT4& MoveRight(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& RightVector, const float& speed) = 0;

	// 정면 이동
	virtual DirectX::XMFLOAT4& MoveForward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed) = 0;

	// 후면 이동
	virtual DirectX::XMFLOAT4& MoveBackward(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& ForwardVector, const float& speed) = 0;

	// 움직임 속도 계산
	virtual const float& ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown) = 0;

	// 회전 속도 계산
	virtual const float& ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown) = 0;
};

