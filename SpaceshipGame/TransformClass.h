#pragma once
class TransformClass
{
public:
	TransformClass();
	TransformClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	~TransformClass();

	HRESULT ChangePosition(const MoveState& state, const bool& IsKeyDown);
	HRESULT ChangeRotation(const RotationState& state, const bool& IsMouseMove);

	// Getter //

	inline const DirectX::XMMATRIX& GetAffine() const { return m_Affine; }
	inline const DirectX::XMFLOAT4& GetPosition() const { return m_Position; }
	inline const DirectX::XMFLOAT4& GetRotation() const { return m_Rotation; }

	// Setter //

	inline void SetFrameTime(const float& time) { m_FrameTime = time; }

	void SetPosition(const DirectX::XMFLOAT4& pos);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetRotation(const DirectX::XMFLOAT4& rot);
	void SetRotation(const float& x, const float& y, const float& z);
	
private:
	void Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);

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

public:
	TransformClass(const TransformClass& other) = delete;
};

