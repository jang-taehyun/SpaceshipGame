#include "pch.h"
#include "LightShaderClass.h"

LightShaderClass::LightShaderClass() {}
LightShaderClass::LightShaderClass(const LightShaderClass& other) {}
LightShaderClass::~LightShaderClass() {}

bool LightShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture, DirectX::XMFLOAT3 LightDirection, DirectX::XMFLOAT4 AmbientColor, DirectX::XMFLOAT4 DiffuseColor, DirectX::XMFLOAT3 CameraPosition, DirectX::XMFLOAT4 SpecularColor, float SpecularPower)
{
	if (!SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, Texture, LightDirection, AmbientColor, DiffuseColor, CameraPosition, SpecularColor, SpecularPower))
	{
		return false;
	}

	RenderShader(DeviceContext, IndexCount);

	return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device* Device, HWND hwnd, const ShaderFileInfo& const info)
{
	ID3D10Blob* ErrorMessage = nullptr;

	// vertex shader code 컴파일 //
	ID3D10Blob* VertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(info.vsFileName, NULL, NULL, info.vsEntryPoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.vsFileName);
		}
		else
		{
			MessageBox(hwnd, info.vsFileName, _T("vertex shader file이 없습니다."), MB_OK);
		}

		return false;
	}

	// pixel shader code 컴파일 //
	ID3D10Blob* PixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(info.psFileName, NULL, NULL, info.psEntryPoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.psFileName);
		}
		else
		{
			MessageBox(hwnd, info.psFileName, _T("Missing pixel shader file"), MB_OK);
		}

		return false;
	}

	// vertex shader 생성 //
	if (FAILED(Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader)))
	{
		return false;
	}

	// pixel shader 생성 //
	if (FAILED(Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader)))
	{
		return false;
	}

	// vertex input layout 생성 //
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
	if (FAILED(Device->CreateInputLayout(PolygonLayout, ElementsCount, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &m_Layout)))
	{
		return false;
	}

	// vertex shader buffer, pixel shader buffer 해제 //
	VertexShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = nullptr;

	// 행렬 상수 버퍼 생성 //
	D3D11_BUFFER_DESC MatrixBufferDesc;
	memset(&MatrixBufferDesc, 0, sizeof(MatrixBufferDesc));

	// 행렬 상수 버퍼 설정
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	// 행렬 상수 버퍼 생성
	if (FAILED(Device->CreateBuffer(&MatrixBufferDesc, NULL, &m_MatrixBuffer)))
	{
		return false;
	}

	// 광원 상수 버퍼(light constant buffer) 생성 //
	D3D11_BUFFER_DESC LightBufferDesc;
	memset(&LightBufferDesc, 0, sizeof(LightBufferDesc));

	// 광원 상수 버퍼 설정
	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.ByteWidth = sizeof(LightBufferType);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	// 광원 상수 버퍼 생성
	if (FAILED(Device->CreateBuffer(&LightBufferDesc, NULL, &m_LightBuffer)))
	{
		return false;
	}

	// 카메라 동적 상수 버퍼(Camera constant buffer) 생성 //
	D3D11_BUFFER_DESC CameraBufferDesc;
	memset(&CameraBufferDesc, 0, sizeof(CameraBufferDesc));

	// 카메라 동적 상수 버퍼 설정
	CameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	CameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CameraBufferDesc.MiscFlags = 0;
	CameraBufferDesc.StructureByteStride = 0;

	// 카메라 동적 상수 버퍼 생성
	if (FAILED(Device->CreateBuffer(&CameraBufferDesc, NULL, &m_CameraBuffer)))
	{
		return false;
	}

	
	// texture sampler state 생성 //
	D3D11_SAMPLER_DESC SamplerDesc;
	memset(&SamplerDesc, 0, sizeof(SamplerDesc));

	// texture sampler state 설정
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// texture sampler state 생성
	if (FAILED(Device->CreateSamplerState(&SamplerDesc, &m_SampleState)))
		return false;

	return true;
}

void LightShaderClass::ShutdownShader()
{
	if (m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState = nullptr;
	}

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

	if (m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = nullptr;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = nullptr;
	}

	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = nullptr;
	}

	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = nullptr;
	}
}

bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture, DirectX::XMFLOAT3 LightDirection, DirectX::XMFLOAT4 AmbientColor, DirectX::XMFLOAT4 DiffuseColor, DirectX::XMFLOAT3 CameraPosition, DirectX::XMFLOAT4 SpecularColor, float SpecularPower)
{
	// 행렬들을 HLSL에 맞게 변환 //
	// 행렬들을 transpose 연산하여 shader에서 사용할 수 있도록 한다.
	WorldMatrix = DirectX::XMMatrixTranspose(WorldMatrix);
	ViewMatrix = DirectX::XMMatrixTranspose(ViewMatrix);
	ProjectionMatrix = DirectX::XMMatrixTranspose(ProjectionMatrix);

	// 행렬 상수 버퍼의 내용 업데이트 //
	// 행렬 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// 행렬 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	MatrixBufferType* DataPtr = (MatrixBufferType*)MappedResource.pData;

	// 행렬 상수 버퍼에 데이터(행렬) 복사
	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	// 행렬 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_MatrixBuffer, 0);

	// vertex shader에서 상수 버퍼의 위치 설정
	// vertex shader의 0번 슬롯에 바인드
	unsigned int BufferNumber = 0;

	// vertex shader의 행렬 상수 버퍼를 업데이트한 것으로 바꾼다.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &m_MatrixBuffer);


	// 광원 상수 버퍼(light constant buffer)의 내용 업데이트 //
	// 광원 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	if (FAILED(DeviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// 광원 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	LightBufferType* DataPtr2 = (LightBufferType*)MappedResource.pData;

	// 광원 상수 버퍼에 데이터(행렬) 복사
	DataPtr2->AmbientColor = AmbientColor;
	DataPtr2->DiffuseColor = DiffuseColor;
	DataPtr2->LightDirection = LightDirection;
	DataPtr2->SpecularColor = SpecularColor;
	DataPtr2->SpecularPower = SpecularPower;

	// 광원 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_LightBuffer, 0);

	// pixel shader에서 광원 상수 버퍼의 위치 설정
	// pixel shader의 0번 슬롯에 바인드
	BufferNumber = 0;

	// pixel shader의 광원 상수 버퍼를 업데이트한 것으로 바꾼다.
	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &m_LightBuffer);


	// 카메라 동적 상수 버퍼(Camera constant buffer)의 내용 업데이트 //
	// 카메라 동적 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	if (FAILED(DeviceContext->Map(m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// 카메라 동적 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	CameraBufferType* DataPtr3 = (CameraBufferType*)MappedResource.pData;

	// 카메라 동적 상수 버퍼에 데이터(행렬) 복사
	DataPtr3->CameraPosition = CameraPosition;
	DataPtr3->padding = 0.f;

	// 카메라 동적 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_CameraBuffer, 0);

	// vertex shader에서 광원 상수 버퍼의 위치 설정
	// vertex shader의 1번 슬롯에 바인드
	BufferNumber = 1;

	// pixel shader의 광원 상수 버퍼를 업데이트한 것으로 바꾼다.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &m_CameraBuffer);


	// pixel shader에서 사용할 shader texture resource(Texture2D) 설정 //
	// GPU 파이프라인에 텍스처 데이터를 바인드
	DeviceContext->PSSetShaderResources(0, 1, &Texture);

	return true;
}