#include "pch.h"
#include "AlphaMapShaderClass.h"

AlphaMapShaderClass::AlphaMapShaderClass() {}
AlphaMapShaderClass::AlphaMapShaderClass(const AlphaMapShaderClass& other) {}
AlphaMapShaderClass::~AlphaMapShaderClass() {}

bool AlphaMapShaderClass::Initialize(ID3D11Device* Device, HWND hwnd)
{
	return InitializeShader(Device, hwnd, _T("../Engine/alphamap.vs"), _T("../Engine/alphamap.ps"));
}

void AlphaMapShaderClass::Shutdown()
{
	ShutdownShader();
}

bool AlphaMapShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView** TextureArray)
{
	if (!SetShaderParameters(DeviceContext, WorldMatrix, ViewMatrix, ProjectionMatrix, TextureArray))
		return false;

	RenderShader(DeviceContext, IndexCount);

	return true;
}

bool AlphaMapShaderClass::InitializeShader(ID3D11Device* Device, HWND hwnd, const TCHAR* vsFileName, const TCHAR* psFileName)
{
	ID3D10Blob* ErrorMessage = nullptr;

	// vertex shader code ������ //
	ID3D10Blob* VertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(vsFileName, NULL, NULL, "AlphaMapVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, vsFileName);
		}
		else
		{
			MessageBox(hwnd, vsFileName, _T("Missing Shader File"), MB_OK);
		}

		return false;
	}

	// pixel shader code ������ //
	ID3D10Blob* PixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(psFileName, NULL, NULL, "AlphaMapPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, psFileName);
		}
		else
		{
			MessageBox(hwnd, psFileName, _T("Missing Shader File"), MB_OK);
		}

		return false;
	}

	// vertex shader ���� //
	if (FAILED(Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader)))
	{
		return false;
	}

	// pixel shader ���� //
	if (FAILED(Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader)))
	{
		return false;
	}

	// vertex input layout ���� //
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[2];
	memset(PolygonLayout, 0, sizeof(PolygonLayout));

	// vertex input layout ����
	// vertex input layout ������ ModelClass�� VertexType ������ ��ġ�ؾ� ��
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

	// input layout�� ���� ���ϱ�
	unsigned int ElementsCount = sizeof(PolygonLayout) / sizeof(PolygonLayout[0]);

	// input layout ����
	if (FAILED(Device->CreateInputLayout(PolygonLayout, ElementsCount, VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), &m_Layout)))
	{
		return false;
	}

	// vertex shader buffer, pixel shader buffer ���� //
	VertexShaderBuffer->Release();
	VertexShaderBuffer = nullptr;

	PixelShaderBuffer->Release();
	PixelShaderBuffer = nullptr;

	// ��� ��� ���� ���� //
	D3D11_BUFFER_DESC MatrixBufferDesc;
	memset(&MatrixBufferDesc, 0, sizeof(MatrixBufferDesc));

	// ��� ��� ���� ����
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;

	// ��� ��� ���� ����
	if (FAILED(Device->CreateBuffer(&MatrixBufferDesc, NULL, &m_MatrixBuffer)))
	{
		return false;
	}

	// texture sampler state ���� //
	D3D11_SAMPLER_DESC SamplerDesc;
	memset(&SamplerDesc, 0, sizeof(SamplerDesc));

	// texture sampler state ����
	SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamplerDesc.MipLODBias = 0.0f;
	SamplerDesc.MaxAnisotropy = 1;
	SamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamplerDesc.BorderColor[0] = 0;
	SamplerDesc.BorderColor[1] = 0;
	SamplerDesc.BorderColor[2] = 0;
	SamplerDesc.BorderColor[3] = 0;
	SamplerDesc.MinLOD = 0;
	SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// texture sampler state ����
	if (FAILED(Device->CreateSamplerState(&SamplerDesc, &m_SampleState)))
		return false;

	return true;
}

void AlphaMapShaderClass::ShutdownShader()
{
	if (m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState = nullptr;
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

void AlphaMapShaderClass::OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, HWND hwnd, const TCHAR* ShaderFileName)
{
	OutputDebugStringA(reinterpret_cast<const char*>(ErrorMessage->GetBufferPointer()));

	ErrorMessage->Release();
	ErrorMessage = nullptr;

	MessageBox(hwnd, _T("Error compiling shader."), ShaderFileName, MB_OK);
}

bool AlphaMapShaderClass::SetShaderParameters(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView** TextureArray)
{
	// ��ĵ��� HLSL�� �°� ��ȯ //
	// ��ĵ��� transpose �����Ͽ� shader���� ����� �� �ֵ��� �Ѵ�.
	WorldMatrix = DirectX::XMMatrixTranspose(WorldMatrix);
	ViewMatrix = DirectX::XMMatrixTranspose(ViewMatrix);
	ProjectionMatrix = DirectX::XMMatrixTranspose(ProjectionMatrix);

	// ��� ������ ���� ������Ʈ //
	// ��� ������ ������ CPU�� �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����´�.
	MatrixBufferType* DataPtr = (MatrixBufferType*)MappedResource.pData;

	// ��� ���ۿ� ������(���) ����
	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	// ��� ������ ����� Ǭ��.
	DeviceContext->Unmap(m_MatrixBuffer, 0);

	// vertex shader���� ��� ������ ��ġ ����
	// vertex shader�� 0�� ���Կ� ���ε�
	unsigned int BufferNumber = 0;

	// vertex shader�� ��� ���۸� ������Ʈ�� ������ �ٲ۴�.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &m_MatrixBuffer);

	// pixel shader���� ����� shader texture resource(Texture2D) ���� //
	// GPU ���������ο� �ؽ�ó �����͸� ���ε�
	DeviceContext->PSSetShaderResources(0, 3, TextureArray);

	return true;
}

void AlphaMapShaderClass::RenderShader(ID3D11DeviceContext* DeviceContext, int IndexCount)
{
	// vertex input layout ���� //
	DeviceContext->IASetInputLayout(m_Layout);

	// vertex shader�� pixel shader ���� //
	DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	// pixel shader���� ����� sampler state ����(SamplerState) //
	DeviceContext->PSSetSamplers(0, 1, &m_SampleState);

	// ������ //
	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}