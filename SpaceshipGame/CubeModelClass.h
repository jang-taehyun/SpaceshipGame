#pragma once

#include "ModelClass.h"

class CubeModelClass : public ModelClass
{
public:
	explicit CubeModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info, const DirectX::XMFLOAT4& color);
	virtual ~CubeModelClass();

	inline const DirectX::XMFLOAT4& GetColor() const { return m_Color; }
	inline void SetColor(const DirectX::XMFLOAT4& value) { m_Color = value; }

private:
	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) override;
	virtual HRESULT RenderShader(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera) override;
	virtual void ReleaseShader() override;
	virtual void SetBuffers(ID3D11DeviceContext* const& DeviceContext) override;

public:
	CubeModelClass() = delete;
	CubeModelClass(const CubeModelClass& other) = delete;

private:
	DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
};

