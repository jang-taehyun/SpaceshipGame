#pragma once

#include "AffineClass.h"

class CameraClass
{
public:
	explicit CameraClass(const AffineInfo& affine);
	virtual ~CameraClass();

	// Getter //

	inline AffineClass* const& GetTransformObject() const { return m_Affine; }
	inline const DirectX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }

	inline const float GetMoveSpeed() const { return m_Affine->GetMoveSpeed(); }
	inline void SetMoveSpeed(const float value) { m_Affine->SetMoveSpeed(value); }

	inline const float GetKeyboardSensitivity() const { return m_Affine->GetKeyboardSensitivity(); }
	inline void SetKeyboardSensitivity(const float value) { m_Affine->SetKeyboardSensitivity(value); }

	void Render();

private:
	HRESULT Initialize(const AffineInfo& affine);
	void Shutdown();

private:
	AffineClass* m_Affine = nullptr;
	DirectX::XMMATRIX m_ViewMatrix = DirectX::XMMATRIX();

public:
	CameraClass() = delete;
	CameraClass(const CameraClass& other) = delete;
};

