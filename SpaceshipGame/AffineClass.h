#pragma once

class AffineClass
{
public:
	explicit AffineClass();
	explicit AffineClass(const AffineInfo& affine);
	virtual ~AffineClass();

	HRESULT ChangePosition(const MoveState& state, const bool& IsKeyDown);
	void ChangeRotation(const long& MouseX, const long& MouseY);

	// Getter //

	inline const DirectX::XMMATRIX& GetAffine() const { return m_Affine; }
	inline const DirectX::XMFLOAT4& GetPosition() const { return m_Position; }
	inline const DirectX::XMFLOAT4& GetRotation() const { return m_Rotation; }
	inline const DirectX::XMFLOAT4& GetScaling() const { return m_Scaling; }

	inline const DirectX::XMVECTOR& GetForwardVector() const { return m_ForwardVector; }
	inline const DirectX::XMVECTOR& GetRightVector() const { return m_RightVector; }
	inline const DirectX::XMVECTOR& GetUpVector() const { return m_UpVector; }

	inline const float& GetKeyboardSensitivity() const { return m_KeyboardSensitivity; }
	inline const float& GetMouseSensitivity() const { return m_MouseSensitivity; }

	inline const float GetMoveSpeed() const { return m_ForwardMoveSpeed; }

	// Setter //

	inline void SetFrameTime(const float& time) { m_FrameTime = time; }
	inline void SetKeyboardSensitivity(const float& value) { m_KeyboardSensitivity = value; }
	inline void SetMouseSensitivity(const float& value) { m_MouseSensitivity = value; }

	inline void SetMoveSpeed(const float& value)
	{
		m_ForwardMoveSpeed = value;
		m_BackwardMoveSpeed = value;
		m_LeftMoveSpeed = value;
		m_RightMoveSpeed = value;
	}

	void SetPosition(const DirectX::XMFLOAT4& pos);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetRotation(const DirectX::XMFLOAT4& rot);
	void SetRotation(const float& x, const float& y, const float& z);
	void SetScale(const DirectX::XMFLOAT4& scale);
	void SetScale(const float& x, const float& y, const float& z);
	
private:
	void Initialize(const AffineInfo& affine);

	void UpdateAffineMatrix();

	void MoveLeft(const bool& IsKeyDown);
	void MoveRight(const bool& IsKeyDown);
	void MoveForward(const bool& IsKeyDown);
	void MoveBackward(const bool& IsKeyDown);

private:
	DirectX::XMFLOAT4 m_Position = { 0.f, 0.f, 0.f, 1.f };
	DirectX::XMFLOAT4 m_Rotation = { 1.f, 1.f, 1.f, 1.f };
	DirectX::XMFLOAT4 m_Scaling = { 1.f, 1.f, 1.f, 1.f };

	DirectX::XMVECTOR m_ForwardVector;							// 로컬 좌표계의 z, x, y 축
	DirectX::XMVECTOR m_RightVector;
	DirectX::XMVECTOR m_UpVector;

	DirectX::XMMATRIX m_Affine = DirectX::XMMatrixIdentity();

	float m_FrameTime = 0.f;

	float m_ForwardMoveSpeed = 0.f;
	float m_BackwardMoveSpeed = 0.f;
	float m_LeftMoveSpeed = 0.f;
	float m_RightMoveSpeed = 0.f;

	float m_KeyboardSensitivity = 0.1f;
	float m_MouseSensitivity = 0.0015f;

public:
	AffineClass(const AffineClass& other) = delete;
};

