#pragma once

#include "pch.h"
#include "ShaderClass.h"

template<typename ShaderBuffers>
Graphic::Shader::ShaderClass<ShaderBuffers>::ShaderClass(ID ShaderID) : m_ShaderID(ShaderID) {}

template<typename ShaderBuffers>
Graphic::Shader::ShaderClass<ShaderBuffers>::ShaderClass(const ShaderClass& other) : m_ShaderID(other.m_ShaderID)
{
	other.m_VertexShader.CopyTo(m_VertexShader.GetAddressOf());
	other.m_PixelShader.CopyTo(m_PixelShader.GetAddressOf());
	other.m_Layout.CopyTo(m_Layout.GetAddressOf());
	other.m_SampleState.CopyTo(m_SampleState.GetAddressOf());
}

template<typename ShaderBuffers>
Graphic::Shader::ShaderClass<ShaderBuffers>::ShaderClass(ShaderClass&& other) noexcept : m_ShaderID(other.m_ShaderID)
{
	m_VertexShader = std::move(other.m_VertexShader);
	m_PixelShader = std::move(other.m_PixelShader);
	m_Layout = std::move(other.m_Layout);
	m_SampleState = std::move(other.m_SampleState);
}

template<typename ShaderBuffers>
Graphic::Shader::ShaderClass<ShaderBuffers>& Graphic::Shader::ShaderClass<ShaderBuffers>::operator=(const ShaderClass& other)
{
	if (this == &other)
		return *this;

	m_ShaderID = other.m_ShaderID;

	other.m_VertexShader.CopyTo(m_VertexShader.ReleaseAndGetAddressOf());
	other.m_PixelShader.CopyTo(m_PixelShader.ReleaseAndGetAddressOf());
	other.m_Layout.CopyTo(m_Layout.ReleaseAndGetAddressOf());
	other.m_SampleState.CopyTo(m_SampleState.ReleaseAndGetAddressOf());

	return *this;
}

template<typename ShaderBuffers>
Graphic::Shader::ShaderClass<ShaderBuffers>& Graphic::Shader::ShaderClass<ShaderBuffers>::operator=(ShaderClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ShaderID = other.m_ShaderID;

	m_VertexShader.Reset();
	m_PixelShader.Reset();
	m_Layout.Reset();
	m_SampleState.Reset();

	m_VertexShader = std::move(other.m_VertexShader);
	m_PixelShader = std::move(other.m_PixelShader);
	m_Layout = std::move(other.m_Layout);
	m_SampleState = std::move(other.m_SampleState);

	return *this;
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::Initialize(HWND hwnd, ID3D11Device* Device, const Loader::ShaderFileInfo& info, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;

	// shader, input layout 초기화
	result = InitializeShaderInputLayout(hwnd, Device, info, VertexDataSemantics);
	assert(SUCCEEDED(result));

	// shader에서 사용하는 buffer들 생성
	result = CreateBuffers(Device);

	// texture sampler state 생성
	result = CreateTextureSamplerState(Device);

	return result;
}

template<typename ShaderBuffers>
void Graphic::Shader::ShaderClass<ShaderBuffers>::BeginRender(ID3D11DeviceContext* DeviceContext)
{
	BindShaderAndInputLayout(DeviceContext);
	SetShaderBuffers(DeviceContext);
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, int InstanceCount, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& Material)
{
	HRESULT result = S_OK;

	// pixel shader에서 사용할 shader texture resource(Texture2D) 설정 //
	// GPU 파이프라인에 텍스처 데이터를 바인드
	for(int i=0; i<Material.size(); ++i)
		DeviceContext->PSSetShaderResources(i, 1, Material[i].GetAddressOf());

	// 렌더링 //
	DeviceContext->DrawIndexedInstanced(IndexCount, InstanceCount, 0, 0, 0);

	return result;
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::InitializeShaderInputLayout(HWND hwnd, ID3D11Device* Device, const Loader::ShaderFileInfo& info, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;
	Microsoft::WRL::ComPtr<ID3D10Blob> ErrorMessage = nullptr;				// shader compile 에러메세지
	Microsoft::WRL::ComPtr<ID3D10Blob> VertexShaderBuffer = nullptr;		// vertex shader buffer
	Microsoft::WRL::ComPtr<ID3D10Blob> PixelShaderBuffer = nullptr;			// pixel shader buffer
	UINT flag = D3D10_SHADER_ENABLE_STRICTNESS;

#ifdef _DEBUG
	D3D_SHADER_MACRO macros[2] =
	{
		{ "_DEBUG", "1" },
		{ nullptr, nullptr }
	};
#endif // _DEBUG

	// shader 정보 확인 //
	assert(Device && hwnd && info.vsFileName != _T("") && info.psFileName != _T("") && info.vsEntryPoint != "" && info.psEntryPoint != "");

	// vertex shader code 컴파일 //
	result = D3DCompileFromFile(info.vsFileName.c_str(),
#ifdef _DEBUG
		macros,
#else
		NULL,
#endif
		NULL, info.vsEntryPoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, VertexShaderBuffer.GetAddressOf(), ErrorMessage.GetAddressOf());
	if (FAILED(result))
	{
		assert(ErrorMessage);
		OutputShaderErrorMessage(hwnd, ErrorMessage.Get(), info.vsFileName);

		return result;
	}

	// pixel shader code 컴파일 //
	result = D3DCompileFromFile(info.psFileName.c_str(),
#ifdef _DEBUG
		macros,
#else
		NULL,
#endif
		NULL, info.psEntryPoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, PixelShaderBuffer.GetAddressOf(), ErrorMessage.GetAddressOf());
	if (FAILED(result))
	{
		assert(ErrorMessage);
		OutputShaderErrorMessage(hwnd, ErrorMessage.Get(), info.psFileName);

		return result;
	}

	// vertex shader 생성 //
	result = Device->CreateVertexShader(VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	assert(SUCCEEDED(result));

	// pixel shader 생성 //
	result =Device->CreatePixelShader(PixelShaderBuffer->GetBufferPointer(), PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	assert(SUCCEEDED(result));

	// input layout 생성
	result = CreateInputLayout(Device, VertexShaderBuffer.Get(), VertexDataSemantics);

	return result;
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;
	std::vector<D3D11_INPUT_ELEMENT_DESC> LayoutDesc;
	D3D11_INPUT_ELEMENT_DESC desc = {};
	std::string semantic;

	// vertex input layout 설정
	// vertex input layout 설정는 ModelClass의 VertexType 구조, vertex shader 내부의 VertexInputType 모두 일치해야 함
	desc.SemanticIndex = 0;
	desc.InputSlot = 0;
	desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.InstanceDataStepRate = 0;

	for (UINT i = 0; i < VertexDataSemantics.size(); ++i)
	{
		desc.SemanticName = VertexDataSemantics[i].c_str();

		if (VertexDataSemantics[i] == "POSITION")
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		else if (VertexDataSemantics[i] == "TEXCOORD")
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		else
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		desc.AlignedByteOffset = (i ? D3D11_APPEND_ALIGNED_ELEMENT : 0);

		LayoutDesc.push_back(desc);
	}

	// instance layout 설정
	// instance layout은 world matrix만 있으므로 따로 처리
	desc.SemanticName = "INSTANCE_WORLD_COLUMN";
	desc.SemanticIndex = 0;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.InputSlot = 1;
	desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
	desc.AlignedByteOffset = 0;
	desc.InstanceDataStepRate = 1;
	LayoutDesc.push_back(desc);

	desc.SemanticName = "INSTANCE_WORLD_COLUMN";
	desc.SemanticIndex = 1;
	desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	LayoutDesc.push_back(desc);

	desc.SemanticName = "INSTANCE_WORLD_COLUMN";
	desc.SemanticIndex = 2;
	LayoutDesc.push_back(desc);

	desc.SemanticName = "INSTANCE_WORLD_COLUMN";
	desc.SemanticIndex = 3;
	LayoutDesc.push_back(desc);

#ifdef _DEBUG
	desc.SemanticName = "INSTANCE_WORLD_COLUMN";
	desc.SemanticIndex = 4;
	LayoutDesc.push_back(desc);
#endif // DEBUG

	// input layout 생성
	result = Device->CreateInputLayout(LayoutDesc.data(), static_cast<UINT>(LayoutDesc.size()), VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), m_Layout.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::CreateTextureSamplerState(ID3D11Device* Device)
{
	HRESULT result = S_OK;
	D3D11_SAMPLER_DESC SamplerDesc = {};								// texture sampler state 설정 정보

	// texture sampler state 생성 //
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
	result = Device->CreateSamplerState(&SamplerDesc, m_SampleState.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

template<typename ShaderBuffers>
void Graphic::Shader::ShaderClass<ShaderBuffers>::OutputShaderErrorMessage(HWND hwnd, ID3D10Blob* ErrorMessage, const std::wstring& ShaderFileName)
{
	const char* msg = reinterpret_cast<const char*>(ErrorMessage->GetBufferPointer());
	int len = static_cast<UINT>(ErrorMessage->GetBufferSize());
	wchar_t wmsg[2000] = { 0, };

	MultiByteToWideChar(CP_ACP, 0, msg, len, wmsg, sizeof(wmsg));

	MessageBox(hwnd, wmsg, ShaderFileName.c_str(), MB_OK);
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize)
{
	HRESULT result = S_OK;
	D3D11_BUFFER_DESC ConstantBufferDesc = {};							// 상수 버퍼 정보

	// 상수 버퍼 설정
	ConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstantBufferDesc.ByteWidth = BufferSize;
	ConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstantBufferDesc.MiscFlags = 0;
	ConstantBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 생성
	result = Device->CreateBuffer(&ConstantBufferDesc, NULL, Buffer);
	assert(SUCCEEDED(result));

	return result;
}

template<typename ShaderBuffers>
void Graphic::Shader::ShaderClass<ShaderBuffers>::BindShaderAndInputLayout(ID3D11DeviceContext* DeviceContext)
{
	// vertex input layout 설정 //
	DeviceContext->IASetInputLayout(m_Layout.Get());

	// vertex shader와 pixel shader 설정 //
	DeviceContext->VSSetShader(m_VertexShader.Get(), NULL, 0);
	DeviceContext->PSSetShader(m_PixelShader.Get(), NULL, 0);

	// pixel shader에서 사용할 sampler state 설정(SamplerState) //
	DeviceContext->PSSetSamplers(0, 1, m_SampleState.GetAddressOf());
}
