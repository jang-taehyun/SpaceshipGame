#pragma once

#include "AffineClass.h"

class CameraClass
{
public:
	explicit CameraClass(const AffineInfo& affine);
	virtual ~CameraClass();

	// Getter //

	inline AffineClass* const& GetAffineObject() const { return m_Affine; }
	inline const DirectX::XMMATRIX& GetViewMatrix() const { return m_ViewMatrix; }

	inline const float GetMoveSpeed() const { return m_Affine->GetMoveSpeed(); }
	inline void SetMoveSpeed(const float value) { m_Affine->SetMoveSpeed(value); }

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

