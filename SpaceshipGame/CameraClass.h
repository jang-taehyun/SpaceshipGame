#pragma once

#include "TransformClass.h"

class CameraClass
{
public:
	CameraClass(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& rotation, const DirectX::XMFLOAT4& scaling);
	~CameraClass();

	// Getter //

	inline TransformClass* const& GetTransformObject() const { return m_Transform; }
	inline const DirectX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }

	inline const float GetMoveSpeed() const { return m_Transform->GetMoveSpeed(); }
	inline void SetMoveSpeed(const float value) { m_Transform->SetMoveSpeed(value); }

	inline const float GetKeyboardSensitivity() const { return m_Transform->GetKeyboardSensitivity(); }
	inline void SetKeyboardSensitivity(const float value) { m_Transform->SetKeyboardSensitivity(value); }

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

