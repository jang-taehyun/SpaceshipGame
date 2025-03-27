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

	inline PositionClass* const& GetTransformObject() const { return m_Position; }
	inline void GetViewMatrix(DirectX::XMMATRIX& ViewMatrix) const { ViewMatrix = m_ViewMatrix; }

	void Render();

private:
	PositionClass* m_Position = nullptr;
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();
};

