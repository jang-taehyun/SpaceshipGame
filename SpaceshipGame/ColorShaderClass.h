#pragma once

#include "ShaderClass.h"

class ColorShaderClass : public ShaderClass
{
private:
	struct ColorBufferType
	{
		DirectX::XMFLOAT4 color;
	};

public:
	explicit ColorShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~ColorShaderClass();

	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const DirectX::XMFLOAT4 color);

private:
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const DirectX::XMFLOAT4 color);
	void ShutdownBuffer();
	HRESULT UpdateColorBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT4& color);

private:
	ID3D11Buffer* m_ColorBuffer = nullptr;

public:
	ColorShaderClass() = delete;
	ColorShaderClass(const ColorShaderClass& other) = delete;
};

