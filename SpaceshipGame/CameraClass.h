#pragma once

class TransformClass;

class CameraClass
{
public:
	CameraClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	~CameraClass();

	// Getter //

	inline TransformClass* const& GetTransformObject() const { return m_Transform; }
	inline const DirectX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }
	
	HRESULT Move(const MoveState& dir);

	void Render();

private:
	HRESULT Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	void Shutdown();

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

private:
	TransformClass* m_Transform = nullptr;
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();

	DirectX::XMVECTOR m_ForwardVector;						// 카메라의 로컬 좌표계의 z, x, y 축
	DirectX::XMVECTOR m_RightVector;
	DirectX::XMVECTOR m_UpVector;

	float m_MoveSpeed = 0.05f;

public:
	CameraClass() = delete;
	CameraClass(const CameraClass& other) = delete;
};

