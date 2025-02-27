#pragma once

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	DirectX::XMFLOAT3 m_Position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 m_Rotation = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();
};

