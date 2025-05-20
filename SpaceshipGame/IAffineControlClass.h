#pragma once

/**
* IAffineControlClass 개요
* - AffineClass의 interface
* - Affine 관련 연산 실행
*	- forward, backward, left, right 이동 실행
*	- rotate 실행
*	- affine matrix 연산 및 리턴
* - speed, sensitivity 관리
*/

class AffineClass;

class IAffineControlClass
{
public:
	explicit IAffineControlClass(const AffineInfo& affine);
	virtual ~IAffineControlClass();

	DirectX::XMFLOAT4X4& GetAffineMatrix() const;

	void MoveActor(const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	void RotateActor(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

	inline const float& GetMoveSpeed() const { return m_MoveSpeed; }
	inline const float& GetRoteteSpeed() const { return m_RotateSpeed; }

	inline const float& GetKeyboardSensitivity() const { return m_KeyboardSensitivity; }
	inline const float& GetMouseSensitivity() const { return m_MouseSensitivity; }

	inline void SetMoveSpeed(const float& value) { m_MoveSpeed = value; }
	inline void SetRoteteSpeed(const float& value) { m_RotateSpeed = value; }

	inline void SetKeyboardSensitivity(const float& value) { m_KeyboardSensitivity = value; }
	inline void SetMouseSensitivity(const float& value) { m_MouseSensitivity = value; }

private:
	HRESULT Initailize(const AffineInfo& affine);
	void Shutdown();

	DirectX::XMFLOAT4& MoveLeft(const DirectX::XMVECTOR& RightVector);
	DirectX::XMFLOAT4& MoveRight(const DirectX::XMVECTOR& RightVector);
	DirectX::XMFLOAT4& MoveForward(const DirectX::XMVECTOR& ForwardVector);
	DirectX::XMFLOAT4& MoveBackward(const DirectX::XMVECTOR& ForwardVector);

	void ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown);
	void ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown);

private:
	AffineClass* m_affine = nullptr;

	float m_KeyboardSensitivity = 0.1f;
	float m_MouseSensitivity = 0.0015f;

	float m_RotateSpeed = 0.f;
	float m_MoveSpeed = 0.f;

public:
	IAffineControlClass() = delete;
	IAffineControlClass(const IAffineControlClass& other) = delete;
};

