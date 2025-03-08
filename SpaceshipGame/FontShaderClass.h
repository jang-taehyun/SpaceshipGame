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
	FontShaderClass();
	FontShaderClass(const FontShaderClass& other);
	~FontShaderClass();

	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor);

private:
	virtual HRESULT InitializeShader(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info) override;
	virtual void ShutdownShader() override;
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor);

	HRESULT UpdatePixelBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT4& PixelColor);

private:
	ID3D11Buffer* m_PixelBuffer = nullptr;
};

