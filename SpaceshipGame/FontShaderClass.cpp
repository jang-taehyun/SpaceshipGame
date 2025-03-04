#include "pch.h"
#include "FontShaderClass.h"

FontShaderClass::FontShaderClass() {}
FontShaderClass::FontShaderClass(const FontShaderClass& other) {}
FontShaderClass::~FontShaderClass() {}


HRESULT FontShaderClass::Render(ID3D11DeviceContext* const DeviceContext, const int& const IndexCount, const DirectX::XMMATRIX& const WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& const ProjectionMatrix, const int& const TextureNum, ID3D11ShaderResourceView** const TextureArray, const DirectX::XMFLOAT4& const PixelColor)
{
	if (FAILED(SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, TextureNum, TextureArray,PixelColor)))
	{
		return E_FAIL;
	}

	RenderShader(DeviceContext, IndexCount);

	return S_OK;
}

HRESULT FontShaderClass::InitializeShader(ID3D11Device* const Device, const HWND const hwnd, const ShaderFileInfo& const info)
{
	if (FAILED(ShaderClass::InitializeShader(Device, hwnd, info)))
	{
		return E_FAIL;
	}

	if (FAILED(CreateConstantBuffer(Device, m_PixelBuffer, sizeof(PixelBufferType))))
	{
		return E_FAIL;
	}

	return S_OK;
}

void FontShaderClass::ShutdownShader()
{
	if (m_PixelBuffer)
	{
		m_PixelBuffer->Release();
		m_PixelBuffer = nullptr;
	}

	ShaderClass::ShutdownShader();
}


HRESULT FontShaderClass::SetShaderParameters(ID3D11DeviceContext* const DeviceContext, const DirectX::XMMATRIX& const WorldMatrix, const DirectX::XMMATRIX& const ViewMatrix, const DirectX::XMMATRIX& const ProjectionMatrix, const int& const TextureNum, ID3D11ShaderResourceView** const TextureArray, const DirectX::XMFLOAT4& const PixelColor)
{
	if (FAILED(ShaderClass::SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, TextureNum, TextureArray)))
	{
		return E_FAIL;
	}

	// pixel 상수 버퍼의 내용 업데이트 //
	// pixel shader에서 pixel constant buffer의 위치 : 0번
	if (FAILED(UpdatePixelBuffer(DeviceContext, 0, PixelColor)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT FontShaderClass::UpdatePixelBuffer(ID3D11DeviceContext* const DeviceContext, unsigned int slot, const DirectX::XMFLOAT4& const PixelColor)
{
	// pixel 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_PixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return E_FAIL;
	}

	// pixel 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	PixelBufferType* DataPtr = (PixelBufferType*)MappedResource.pData;

	// pixel 상수 버퍼에 데이터 복사
	DataPtr->PixelColor = PixelColor;

	// pixel 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_PixelBuffer, 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 matrix constant buffer의 내용 업데이트
	DeviceContext->PSSetConstantBuffers(slot, 1, &m_PixelBuffer);

	return S_OK;
}
