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

	// vertex shader code ������ //
	ID3D10Blob* VertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(info.vsFileName, NULL, NULL, info.vsEntryPoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.vsFileName);
		}
		else
		{
			MessageBox(hwnd, info.vsFileName, _T("vertex shader file�� �����ϴ�."), MB_OK);
		}

		return false;
	}

	// pixel shader code ������ //
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
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
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

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

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

	// ���� ��� ����(light constant buffer) ���� //
	D3D11_BUFFER_DESC LightBufferDesc;
	memset(&LightBufferDesc, 0, sizeof(LightBufferDesc));

	// ���� ��� ���� ����
	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.ByteWidth = sizeof(LightBufferType);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;
	LightBufferDesc.StructureByteStride = 0;

	// ���� ��� ���� ����
	if (FAILED(Device->CreateBuffer(&LightBufferDesc, NULL, &m_LightBuffer)))
	{
		return false;
	}

	// ī�޶� ���� ��� ����(Camera constant buffer) ���� //
	D3D11_BUFFER_DESC CameraBufferDesc;
	memset(&CameraBufferDesc, 0, sizeof(CameraBufferDesc));

	// ī�޶� ���� ��� ���� ����
	CameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	CameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	CameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CameraBufferDesc.MiscFlags = 0;
	CameraBufferDesc.StructureByteStride = 0;

	// ī�޶� ���� ��� ���� ����
	if (FAILED(Device->CreateBuffer(&CameraBufferDesc, NULL, &m_CameraBuffer)))
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
	SamplerDesc.MipLODBias = 0.f;
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
	// ��ĵ��� HLSL�� �°� ��ȯ //
	// ��ĵ��� transpose �����Ͽ� shader���� ����� �� �ֵ��� �Ѵ�.
	WorldMatrix = DirectX::XMMatrixTranspose(WorldMatrix);
	ViewMatrix = DirectX::XMMatrixTranspose(ViewMatrix);
	ProjectionMatrix = DirectX::XMMatrixTranspose(ProjectionMatrix);

	// ��� ��� ������ ���� ������Ʈ //
	// ��� ��� ������ ������ CPU�� �� �� �ֵ��� ���
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// ��� ��� ������ �����Ϳ� ���� �����͸� �����´�.
	MatrixBufferType* DataPtr = (MatrixBufferType*)MappedResource.pData;

	// ��� ��� ���ۿ� ������(���) ����
	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	// ��� ��� ������ ����� Ǭ��.
	DeviceContext->Unmap(m_MatrixBuffer, 0);

	// vertex shader���� ��� ������ ��ġ ����
	// vertex shader�� 0�� ���Կ� ���ε�
	unsigned int BufferNumber = 0;

	// vertex shader�� ��� ��� ���۸� ������Ʈ�� ������ �ٲ۴�.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &m_MatrixBuffer);


	// ���� ��� ����(light constant buffer)�� ���� ������Ʈ //
	// ���� ��� ������ ������ CPU�� �� �� �ֵ��� ���
	if (FAILED(DeviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// ���� ��� ������ �����Ϳ� ���� �����͸� �����´�.
	LightBufferType* DataPtr2 = (LightBufferType*)MappedResource.pData;

	// ���� ��� ���ۿ� ������(���) ����
	DataPtr2->AmbientColor = AmbientColor;
	DataPtr2->DiffuseColor = DiffuseColor;
	DataPtr2->LightDirection = LightDirection;
	DataPtr2->SpecularColor = SpecularColor;
	DataPtr2->SpecularPower = SpecularPower;

	// ���� ��� ������ ����� Ǭ��.
	DeviceContext->Unmap(m_LightBuffer, 0);

	// pixel shader���� ���� ��� ������ ��ġ ����
	// pixel shader�� 0�� ���Կ� ���ε�
	BufferNumber = 0;

	// pixel shader�� ���� ��� ���۸� ������Ʈ�� ������ �ٲ۴�.
	DeviceContext->PSSetConstantBuffers(BufferNumber, 1, &m_LightBuffer);


	// ī�޶� ���� ��� ����(Camera constant buffer)�� ���� ������Ʈ //
	// ī�޶� ���� ��� ������ ������ CPU�� �� �� �ֵ��� ���
	if (FAILED(DeviceContext->Map(m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return false;
	}

	// ī�޶� ���� ��� ������ �����Ϳ� ���� �����͸� �����´�.
	CameraBufferType* DataPtr3 = (CameraBufferType*)MappedResource.pData;

	// ī�޶� ���� ��� ���ۿ� ������(���) ����
	DataPtr3->CameraPosition = CameraPosition;
	DataPtr3->padding = 0.f;

	// ī�޶� ���� ��� ������ ����� Ǭ��.
	DeviceContext->Unmap(m_CameraBuffer, 0);

	// vertex shader���� ���� ��� ������ ��ġ ����
	// vertex shader�� 1�� ���Կ� ���ε�
	BufferNumber = 1;

	// pixel shader�� ���� ��� ���۸� ������Ʈ�� ������ �ٲ۴�.
	DeviceContext->VSSetConstantBuffers(BufferNumber, 1, &m_CameraBuffer);


	// pixel shader���� ����� shader texture resource(Texture2D) ���� //
	// GPU ���������ο� �ؽ�ó �����͸� ���ε�
	DeviceContext->PSSetShaderResources(0, 1, &Texture);

	return true;
}