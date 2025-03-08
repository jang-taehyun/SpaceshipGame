#include "pch.h"
#include "FontShaderClass.h"

FontShaderClass::FontShaderClass() {}
FontShaderClass::FontShaderClass(const FontShaderClass& other) {}
FontShaderClass::~FontShaderClass() {}


HRESULT FontShaderClass::Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor)
{
	if (FAILED(SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Textures, PixelColor)))
	{
		return E_FAIL;
	}

	RenderShader(DeviceContext, IndexCount);

	return S_OK;
}

HRESULT FontShaderClass::InitializeShader(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info)
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


HRESULT FontShaderClass::SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor)
{
	unsigned int SlotNum = 0;

	if (FAILED(ShaderClass::SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Textures)))
	{
		return E_FAIL;
	}

	// pixel ��� ������ ���� ������Ʈ //
	// pixel shader���� pixel constant buffer�� ��ġ : 0��
	SlotNum = 0;
	if (FAILED(UpdatePixelBuffer(DeviceContext, SlotNum, PixelColor)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT FontShaderClass::UpdatePixelBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT4& PixelColor)
{
	// pixel ��� ������ ������ CPU�� �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_PixelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return E_FAIL;
	}

	// pixel ��� ������ �����Ϳ� ���� �����͸� �����´�.
	PixelBufferType* DataPtr = (PixelBufferType*)MappedResource.pData;

	// pixel ��� ���ۿ� ������ ����
	DataPtr->PixelColor = PixelColor;

	// pixel ��� ������ ����� Ǭ��.
	DeviceContext->Unmap(m_PixelBuffer, 0);

	// vertex shader���� ��� ������ ��ġ ���� �� matrix constant buffer�� ���� ������Ʈ
	DeviceContext->PSSetConstantBuffers(slot, 1, &m_PixelBuffer);

	return S_OK;
}
