#include "pch.h"
#include "LightShaderClass.h"

LightShaderClass::LightShaderClass() {}
LightShaderClass::LightShaderClass(const LightShaderClass& other) {}
LightShaderClass::~LightShaderClass() {}

HRESULT LightShaderClass::Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT3& CameraPosition, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower)
{
	if (FAILED(SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Textures, LightDirection, AmbientColor, DiffuseColor, CameraPosition, SpecularColor, SpecularPower)))
	{
		return E_FAIL;
	}

	RenderShader(DeviceContext, IndexCount);

	return S_OK;
}

HRESULT LightShaderClass::InitializeShader(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info)
{
	if (FAILED(ShaderClass::InitializeShader(Device, hwnd, info)))
	{
		return E_FAIL;
	}

	// 광원 상수 버퍼(light constant buffer) 생성 //
	if (FAILED(CreateConstantBuffer(Device, m_LightBuffer, sizeof(LightBufferType))))
	{
		return E_FAIL;
	}

	// 카메라 동적 상수 버퍼(Camera constant buffer) 생성 //
	if (FAILED(CreateConstantBuffer(Device, m_CameraBuffer, sizeof(CameraBufferType))))
	{
		return E_FAIL;
	}

	return S_OK;
}

void LightShaderClass::ShutdownShader()
{
	if (m_CameraBuffer)
	{
		m_CameraBuffer->Release();
		m_CameraBuffer = nullptr;
	}

	if (m_LightBuffer)
	{
		m_LightBuffer->Release();
		m_LightBuffer = nullptr;
	}

	ShaderClass::ShutdownShader();
}

HRESULT LightShaderClass::CreateInputLayout(ID3D11Device* const& Device, ID3D10Blob* const& VertexShaderBuffer, ID3D10Blob* const& PixelShaderBuffer)
{
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
	memset(PolygonLayout, 0, sizeof(PolygonLayout));

	// vertex input layout 설정
	// vertex input layout 설정는 ModelClass와 VertexType 구조와 일치해야 함
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

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

	// input layout의 개수 구하기
	unsigned int ElementsCount = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	// input layout 생성
	if (FAILED(Device->CreateInputLayout(PolygonLayout, ElementsCount, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), GetInputLayoutPointer())))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT LightShaderClass::SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT3& CameraPosition, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower)
{
	unsigned int SlotNum = 0;

	if (FAILED(ShaderClass::SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Textures)))
	{
		return E_FAIL;
	}

	// 광원 상수 버퍼(light constant buffer)의 내용 업데이트 //
	// pixel shader에서 광원 상수 버퍼의 위치 : 0번
	SlotNum = 0;
	if (FAILED(UpdateLightBuffer(DeviceContext, SlotNum, LightDirection, AmbientColor, DiffuseColor, SpecularColor, SpecularPower)))
	{
		return E_FAIL;
	}

	// 카메라 동적 상수 버퍼(Camera constant buffer)의 내용 업데이트 //
	// vertex shader에서 카메라 동적 상수 버퍼의 위치 : 1번
	SlotNum = 1;
	if (FAILED(UpdateCameraBuffer(DeviceContext, SlotNum, CameraPosition)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT LightShaderClass::UpdateLightBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower)
{
	// 광원 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return E_FAIL;
	}

	// 광원 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	LightBufferType* DataPtr = (LightBufferType*)MappedResource.pData;

	// 광원 상수 버퍼에 데이터(행렬) 복사
	DataPtr->AmbientColor = AmbientColor;
	DataPtr->DiffuseColor = DiffuseColor;
	DataPtr->LightDirection = LightDirection;
	DataPtr->SpecularColor = SpecularColor;
	DataPtr->SpecularPower = SpecularPower;

	// 광원 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_LightBuffer, 0);

	// pixel shader에서 광원 상수 버퍼의 위치 설정 및 광원 상수 버퍼의 내용 업데이트
	DeviceContext->PSSetConstantBuffers(slot, 1, &m_LightBuffer);

	return S_OK;
}

HRESULT LightShaderClass::UpdateCameraBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT3& CameraPosition)
{
	// 카메라 동적 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return E_FAIL;
	}

	// 카메라 동적 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	CameraBufferType* DataPtr = (CameraBufferType*)MappedResource.pData;

	// 카메라 동적 상수 버퍼에 데이터(행렬) 복사
	DataPtr->CameraPosition = CameraPosition;
	DataPtr->padding = 0.f;

	// 카메라 동적 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_CameraBuffer, 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 카메라 동적 상수 버퍼의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, &m_CameraBuffer);

	return S_OK;
}
