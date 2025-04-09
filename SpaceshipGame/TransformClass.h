#pragma once

class TransformClass
{
public:
	TransformClass();
	TransformClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	~TransformClass();

	HRESULT ChangePosition(const MoveState& state, const bool& IsKeyDown);
	void ChangeRotation(const long& MouseX, const long& MouseY);

	// Getter //

	inline const DirectX::XMMATRIX& GetAffine() const { return m_Affine; }
	inline const DirectX::XMFLOAT4& GetPosition() const { return m_Position; }
	inline const DirectX::XMFLOAT4& GetRotation() const { return m_Rotation; }
	inline const float& GetKeyboardSensitivity() const { return m_KeyboardSensitivity; }

	// Setter //

	inline void SetFrameTime(const float& time) { m_FrameTime = time; }
	inline void SetKeyboardSensitivity(const float& value) { m_KeyboardSensitivity = value; }

	void SetPosition(const DirectX::XMFLOAT4& pos);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetRotation(const DirectX::XMFLOAT4& rot);
	void SetRotation(const float& x, const float& y, const float& z);
	
private:
	void Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);

	void UpdateAffineMatrix();

	void MoveLeft(const bool& IsKeyDown);
	void MoveRight(const bool& IsKeyDown);
	void MoveForward(const bool& IsKeyDown);
	void MoveBackward(const bool& IsKeyDown);

private:
	DirectX::XMFLOAT4 m_Position = { 0.f, 0.f, 0.f, 0.f };
	DirectX::XMFLOAT4 m_Rotation = { 0.f, 0.f, 0.f, 0.f };
	DirectX::XMFLOAT4 m_Scaling = { 0.f, 0.f, 0.f, 0.f };

	DirectX::XMMATRIX m_Affine = DirectX::XMMatrixIdentity();

	float m_FrameTime = 0.f;

	float m_ForwardMoveSpeed = 0.f;
	float m_BackwardMoveSpeed = 0.f;
	float m_LeftMoveSpeed = 0.f;
	float m_RightMoveSpeed = 0.f;

	float m_KeyboardSensitivity = 0.1f;
	float m_MouseSensitivity = 0.0015f;

public:
	TransformClass(const TransformClass& other) = delete;
};

