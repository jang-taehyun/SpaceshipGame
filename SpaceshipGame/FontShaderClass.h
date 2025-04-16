#pragma once

#include "ShaderClass.h"

class FontShaderClass : public ShaderClass
{
private:
	struct PixelBufferType
	{
		DirectX::XMFLOAT4 PixelColor;
	};

public:
	FontShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~FontShaderClass();

	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor);

private:
	virtual HRESULT CreateInputLayout(ID3D11Device* const& Device) override;

	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) override;
	virtual void ShutdownShader() override;
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor);

	HRESULT UpdatePixelBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT4& PixelColor);

	virtual void ShutdownLayout() override;

private:
	ID3D11Buffer* m_PixelBuffer = nullptr;

public:
	FontShaderClass() = delete;
	FontShaderClass(const FontShaderClass& other) = delete;
};

