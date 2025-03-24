#pragma once

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass& other);
	~PositionClass();

	void Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	HRESULT ChangePosition(const MoveState& state, const bool& IsKeyDown);
	HRESULT ChangeRotation(const RotationState& state, const bool& IsMouseMove);

	// Getter //

	inline const DirectX::XMMATRIX& GetAffine() const { return m_Affine; }
	inline const DirectX::XMFLOAT4& GetPosition() const { return m_Position; }
	inline const DirectX::XMFLOAT4& GetRotation() const { return m_Rotation; }

	// Setter //
	inline void SetPosition(const DirectX::XMFLOAT4& pos) { m_Position = pos; }
	inline void SetPosition(const float& x, const float& y, const float& z) { m_Position = DirectX::XMFLOAT4(x, y, z, 1.f); }
	inline void SetRotation(const DirectX::XMFLOAT4& rot) { m_Rotation = rot; }
	inline void SetRotation(const float& x, const float& y, const float& z) { m_Rotation = DirectX::XMFLOAT4(x, y, z, 1.f); }
	inline void SetFrameTime(const float& time) { m_FrameTime = time; }

private:
	void UpdateAffineMatrix();

	void TurnLeft(const bool& IsMouseMove);
	void TurnRight(const bool& IsMouseMove);

private:
	DirectX::XMFLOAT4 m_Position = { 0.f, 0.f, 0.f, 0.f };
	DirectX::XMFLOAT4 m_Rotation = { 0.f, 0.f, 0.f, 0.f };
	DirectX::XMFLOAT4 m_Scaling = { 0.f, 0.f, 0.f, 0.f };

	DirectX::XMMATRIX m_Affine = DirectX::XMMatrixIdentity();

	float m_FrameTime = 0.f;
	float m_RotationY = 0.f;
	float m_LeftTurnSpeed = 0.f;
	float m_RightTurnSpeed = 0.f;
};

