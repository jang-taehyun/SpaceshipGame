#pragma once

class TransformClass;

class CameraClass
{
public:
	CameraClass();
	~CameraClass();

	// Getter //

	inline TransformClass* const& GetTransformObject() const { return m_Transform; }
	inline void GetViewMatrix(DirectX::XMMATRIX& ViewMatrix) const { ViewMatrix = m_ViewMatrix; }

	void Render();

private:
	HRESULT Initialize();
	void Shutdown();

private:
	TransformClass* m_Transform = nullptr;
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();

public:
	CameraClass(const CameraClass& other) = delete;
};

