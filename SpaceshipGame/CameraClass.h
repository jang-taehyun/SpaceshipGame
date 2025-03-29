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
	

	

	void Render();

private:
	HRESULT Initialize(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	void Shutdown();

private:
	TransformClass* m_Transform = nullptr;
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();

public:
	CameraClass() = delete;
	CameraClass(const CameraClass& other) = delete;
};

