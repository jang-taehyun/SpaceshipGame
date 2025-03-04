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

	virtual HRESULT Render(ID3D11DeviceContext* const DeviceContext, const int& const IndexCount, const DirectX::XMMATRIX& const WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& const ProjectionMatrix, const int& const TextureNum, ID3D11ShaderResourceView** const TextureArray, const DirectX::XMFLOAT4& const PixelColor);

private:
	virtual HRESULT InitializeShader(ID3D11Device* const Device, const HWND const hwnd, const ShaderFileInfo& const info);
	virtual void ShutdownShader();
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const DeviceContext, const DirectX::XMMATRIX& const WorldMatrix, const DirectX::XMMATRIX& const ViewMatrix, const DirectX::XMMATRIX& const ProjectionMatrix, const int& const TextureNum, ID3D11ShaderResourceView** const TextureArray, const DirectX::XMFLOAT4& const PixelColor);

	HRESULT UpdatePixelBuffer(ID3D11DeviceContext* const DeviceContext, unsigned int slot, const DirectX::XMFLOAT4& const PixelColor);

private:
	ID3D11Buffer* m_PixelBuffer = nullptr;
};

