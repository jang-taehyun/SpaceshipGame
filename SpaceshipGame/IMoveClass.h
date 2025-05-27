#pragma once

class IMoveClass
{
public:
	virtual ~IMoveClass() = default;

	// 이동
	virtual const DirectX::XMFLOAT4& Move(const DirectX::XMFLOAT4& curPosition, const DirectX::XMFLOAT4& vector, const MoveState& state, const float& frame_time, const bool& IsKeyDown) = 0;
	 
	// 움직임 속도 반환
	virtual inline const float& GetMoveSpeed() const = 0;
	
	// 움직임 속도 세팅
	virtual inline void SetMoveSpeed(const float& value) = 0;

private:

	// 방향 벡터(forward vector, right vector, up vector) 반환
	virtual void GetDirectionVectors(DirectX::XMFLOAT4& forward, DirectX::XMFLOAT4& right, DirectX::XMFLOAT4& up) const = 0;

	// forward vector 반환
	virtual const DirectX::XMFLOAT4& GetForwardVector() const = 0;

	// right vector 반환
	virtual const DirectX::XMFLOAT4& GetRightVector() const = 0;

	// up vector 반환
	virtual const DirectX::XMFLOAT4& GetUpVector() const = 0;

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
};

