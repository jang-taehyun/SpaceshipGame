#include "pch.h"
#include "FontShaderClass.h"

static ErrorContent e;

FontShaderClass::FontShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) : ShaderClass(hwnd, Device, info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("FontShaderClass constructor");

	// input layout 생성 //
	result = CreateInputLayout(Device);
	if (FAILED(result))
	{
		ShutdownShaderBuffer();
		ShutdownLayout();
		ShutdownShader();
		throw e;
	}

	// pixel buffer 생성 //
	result = InitializeShader(hwnd, Device, info);
	if (FAILED(result))
	{
		ShutdownShaderBuffer();
		ShutdownLayout();
		ShutdownShader();
		throw e;
	}

	// vertex shader buffer, pixel shader buffer 해제 //
	ShutdownShaderBuffer();
}

FontShaderClass::~FontShaderClass()
{
	ShutdownLayout();
	ShutdownShader();
}

HRESULT FontShaderClass::Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor)
{
	if (FAILED(SetShaderParameters(DeviceContext, transform, Textures, PixelColor)))
	{
		return E_FAIL;
	}

	RenderShader(DeviceContext, IndexCount);

	return S_OK;
}

HRESULT FontShaderClass::CreateInputLayout(ID3D11Device* const& Device)
{
	HRESULT result = S_OK;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	UINT ElementsCount = 0;

	memset(PolygonLayout, 0, sizeof(PolygonLayout));

	// vertex input layout 설정
	// vertex input layout 설정는 ModelClass의 VertexType 구조, vertex shader 내부의 VertexInputType 모두 일치해야 함
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	// input layout의 개수 구하기
	ElementsCount = static_cast<UINT>(sizeof(PolygonLayout) / sizeof(PolygonLayout[0]));

	// input layout 생성
	result = Device->CreateInputLayout(PolygonLayout, ElementsCount, m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), &m_Layout);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT FontShaderClass::InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
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
}


HRESULT FontShaderClass::SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT4& PixelColor)
{
	unsigned int SlotNum = 0;

	if (FAILED(ShaderClass::SetShaderParameters(DeviceContext, transform, Textures)))
	{
		return E_FAIL;
	}

	// pixel 상수 버퍼의 내용 업데이트 //
	// pixel shader에서 pixel constant buffer의 위치 : 0번
	SlotNum = 0;
	if (FAILED(UpdatePixelBuffer(DeviceContext, SlotNum, PixelColor)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT FontShaderClass::UpdatePixelBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT4& PixelColor)
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

void FontShaderClass::ShutdownLayout()
{
	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = nullptr;
	}
}
