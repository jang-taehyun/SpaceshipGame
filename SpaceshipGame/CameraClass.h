#pragma once

#include "PositionClass.h"

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass& other);
	~CameraClass();

	HRESULT Initialize();

	// Getter //

	inline const DirectX::XMFLOAT4& GetPosition() const { return m_Position->GetPosition(); }
	inline const DirectX::XMFLOAT4& GetRotation() const { return m_Position->GetRotation(); }
	inline void GetViewMatrix(DirectX::XMMATRIX& ViewMatrix) const { ViewMatrix = m_ViewMatrix; }

	// Setter //

	inline void SetPosition(const float& x, const float& y, const float& z) { m_Position->SetPosition(x, y, z); }
	inline void SetRotation(const float& x, const float& y, const float& z) { m_Position->SetRotation(x, y, z); }

	void Render();
private:
	PositionClass* m_Position = nullptr;
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();
};

