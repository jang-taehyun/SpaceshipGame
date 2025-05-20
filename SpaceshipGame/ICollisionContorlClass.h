#pragma once

/**
* ICollisionContorlClass 개요
* - CollisionClass의 interface
* - 충돌 검사
* - Affine 관련 연산 실행
*	- forward, backward, left, right 이동 실행
*	- rotate 실행
*	- affine matrix 연산 및 리턴
* - CollisionClass 인스턴스 반환
*/

#include <SimpleMath.h>
#include <DirectXCollision.h>

class CollisionClass;

class ICollisionContorlClass
{
public:
	explicit ICollisionContorlClass(const AffineInfo& affine);
	virtual ~ICollisionContorlClass();

	const DirectX::ContainmentType& GetCollideState(const DirectX::BoundingOrientedBox* const& collision);
	const DirectX::ContainmentType& GetCollideState(const DirectX::SimpleMath::Ray* const& ray);

	inline const DirectX::BoundingOrientedBox* const& GetCollision() const;

	DirectX::XMFLOAT4X4& GetAffineMatrix() const;

	void MoveCollision(const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	void RotateCollision(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

	inline const float& GetMoveSpeed() const { return m_MoveSpeed; }
	inline const float& GetRoteteSpeed() const { return m_RotateSpeed; }

	inline const float& GetKeyboardSensitivity() const { return m_KeyboardSensitivity; }
	inline const float& GetMouseSensitivity() const { return m_MouseSensitivity; }

	inline void SetMoveSpeed(const float& value) { m_MoveSpeed = value; }
	inline void SetRoteteSpeed(const float& value) { m_RotateSpeed = value; }

	inline void SetKeyboardSensitivity(const float& value) { m_KeyboardSensitivity = value; }
	inline void SetMouseSensitivity(const float& value) { m_MouseSensitivity = value; }

private:
	HRESULT Initialize(const AffineInfo& affine);
	void Shutdown();

	DirectX::XMFLOAT4& MoveLeft(const DirectX::XMVECTOR& RightVector);
	DirectX::XMFLOAT4& MoveRight(const DirectX::XMVECTOR& RightVector);
	DirectX::XMFLOAT4& MoveForward(const DirectX::XMVECTOR& ForwardVector);
	DirectX::XMFLOAT4& MoveBackward(const DirectX::XMVECTOR& ForwardVector);

	void ComputeMoveSpeed(const float& frame_time, const bool& IsKeyDown);
	void ComputeRotateSpeed(const float& frame_time, const bool& IsKeyDown);

private:
	CollisionClass* m_Collision = nullptr;

	float m_KeyboardSensitivity = 0.1f;
	float m_MouseSensitivity = 0.0015f;

	float m_RotateSpeed = 0.f;
	float m_MoveSpeed = 0.f;

public:
	ICollisionContorlClass() = delete;
	ICollisionContorlClass(const ICollisionContorlClass& other) = delete;
};

