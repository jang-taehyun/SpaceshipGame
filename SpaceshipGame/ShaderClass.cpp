#include "pch.h"
#include "ShaderClass.h"

static ErrorContent e;

ShaderClass::ShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass constructor");

	result = Initialize(hwnd, Device, info);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

ShaderClass::~ShaderClass()
{
	Shutdown();
}

HRESULT ShaderClass::Initialize(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass Initialize()");

	// 매개 변수 확인 //
	if (!Device || !hwnd || info.vsFileName == _T("") || info.psFileName == _T("") || info.vsEntryPoint == "" || info.psEntryPoint == "")
	{
		e.contents = _T("매개변수 중 비어있는 매개변수가 존재합니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// shader 초기화 //
	result = InitializeShader(hwnd, Device, info);
	if (FAILED(result))
		return result;

	return result;
}

void ShaderClass::Shutdown()
{
	ShutdownShaderBuffer();
	ShutdownShader();
}

HRESULT ShaderClass::Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures)
{
	if (FAILED(SetShaderParameters(DeviceContext, transform, Textures)))
		return E_FAIL;

	RenderShader(DeviceContext, IndexCount);

	return S_OK;
}

HRESULT ShaderClass::InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
	ID3D10Blob* ErrorMessage = nullptr;

	// vertex shader code 컴파일 //
	if (FAILED(D3DCompileFromFile(info.vsFileName.c_str(), NULL, NULL, info.vsEntryPoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_VertexShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.vsFileName);
		}
		else
		{
			MessageBox(hwnd, info.vsFileName.c_str(), _T("vertex shader file이 없습니다."), MB_OK);
		}

		return E_FAIL;
	}

	// pixel shader code 컴파일 //
	if (FAILED(D3DCompileFromFile(info.psFileName.c_str(), NULL, NULL, info.psEntryPoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_PixelShaderBuffer, &ErrorMessage)))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.psFileName);
		}
		else
		{
			MessageBox(hwnd, info.psFileName.c_str(), _T("Missing pixel shader file"), MB_OK);
		}

		return E_FAIL;
	}

	// vertex shader 생성 //
	if (FAILED(Device->CreateVertexShader(m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader)))
	{
		return E_FAIL;
	}

	// pixel shader 생성 //
	if (FAILED(Device->CreatePixelShader(m_PixelShaderBuffer->GetBufferPointer(), m_PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader)))
	{
		return E_FAIL;
	}

	// 행렬 상수 버퍼 생성 //
	if (FAILED(CreateConstantBuffer(Device, m_MatrixBuffer, sizeof(MatrixBufferType))))
	{
		return E_FAIL;
	}

	// texture sampler state 생성 //
	if (FAILED(CreateTextureSamplerState(Device, m_SampleState)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT ShaderClass::CreateConstantBuffer(ID3D11Device* const& Device, ID3D11Buffer*& Buffer, const UINT& BufferSize)
{
	D3D11_BUFFER_DESC ConstantBufferDesc;
	memset(&ConstantBufferDesc, 0, sizeof(ConstantBufferDesc));

	// 상수 버퍼 설정
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBufferDesc.ByteWidth = BufferSize;
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDesc.MiscFlags = 0;
	ConstantBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 생성
	if (FAILED(Device->CreateBuffer(&ConstantBufferDesc, NULL, &Buffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT ShaderClass::CreateTextureSamplerState(ID3D11Device* const& Device, ID3D11SamplerState*& sampler)
{
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
	if (FAILED(Device->CreateSamplerState(&SamplerDesc, &sampler)))
		return E_FAIL;

	return S_OK;
}

void ShaderClass::ShutdownShader()
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

void ShaderClass::ShutdownShaderBuffer()
{
	if (m_PixelShaderBuffer)
	{
		m_PixelShaderBuffer->Release();
		m_PixelShaderBuffer = nullptr;
	}

	if (m_VertexShaderBuffer)
	{
		m_VertexShaderBuffer->Release();
		m_VertexShaderBuffer = nullptr;
	}
}

void ShaderClass::OutputShaderErrorMessage(ID3D10Blob*& ErrorMessage, const HWND& hwnd, const std::wstring& ShaderFileName)
{
	OutputDebugStringA(reinterpret_cast<const char*>(ErrorMessage->GetBufferPointer()));

	ErrorMessage->Release();
	ErrorMessage = nullptr;

	MessageBox(hwnd, _T("Error compiling shader."), ShaderFileName.c_str(), MB_OK);
}

HRESULT ShaderClass::SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures)
{
	unsigned int SlotNum = 0;

	// 행렬들을 HLSL에 맞게 변환 //
	// 행렬들을 transpose 연산하여 shader에서 사용할 수 있도록 한다.
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranspose(transform.world);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixTranspose(transform.view);
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixTranspose(transform.projection);

	// matrix constant buffer의 내용 업데이트 //
	// vertex shader에서 matrix constant buffer의 위치 : 0번
	SlotNum = 0;
	if (FAILED(UpdateMatrixBuffer(DeviceContext, SlotNum, worldMatrix, viewMatrix, projectionMatrix)))
	{
		return E_FAIL;
	}

	// pixel shader에서 사용할 shader texture resource(Texture2D) 설정 //
	// GPU 파이프라인에 텍스처 데이터를 바인드
	DeviceContext->PSSetShaderResources(0, (UINT)Textures.size(), Textures.data());

	return S_OK;
}

HRESULT ShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix)
{
	// matrix constant buffer의 내용을 CPU가 쓸 수 있도록 잠금
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (FAILED(DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		return E_FAIL;
	}

	// matrix constant buffer의 데이터에 대한 포인터를 가져온다.
	MatrixBufferType* DataPtr = (MatrixBufferType*)MappedResource.pData;

	// matrix constant buffer에 데이터(행렬) 복사
	DataPtr->World = WorldMatrix;
	DataPtr->View = ViewMatrix;
	DataPtr->Projection = ProjectionMatrix;

	// matrix constant buffer의 잠금을 푼다.
	DeviceContext->Unmap(m_MatrixBuffer, 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 matrix constant buffer의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, &m_MatrixBuffer);

	return S_OK;
}

void ShaderClass::RenderShader(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount)
{
	// vertex input layout 설정 //
	DeviceContext->IASetInputLayout(m_Layout);

	// vertex shader와 pixel shader 설정 //
	DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	// pixel shader에서 사용할 sampler state 설정(SamplerState) //
	DeviceContext->PSSetSamplers(0, 1, &m_SampleState);

	// 렌더링 //
	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}
