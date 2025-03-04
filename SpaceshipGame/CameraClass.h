#pragma once

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass& other);
	~CameraClass();

	// Getter //

	inline DirectX::XMFLOAT3 GetPosition() { return m_Position; }
	inline DirectX::XMFLOAT3 GetRotation() { return m_Rotation; }
	inline void GetViewMatrix(DirectX::XMMATRIX& ViewMatrix) { ViewMatrix = m_ViewMatrix; }

	// Setter //

	inline void SetPosition(const float x, const float y, const float z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }
	inline void SetRotation(const float x, const float y, const float z) { m_Rotation.x = x; m_Rotation.y = y; m_Rotation.z = z; }

	void Render();
private:
	DirectX::XMFLOAT3 m_Position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 m_Rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();
};

