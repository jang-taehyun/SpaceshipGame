#include "pch.h"
#include "LightClass.h"
#include "IObjectClass.h"
#include "CameraClass.h"
#include "TypeConverterClass.h"
#include "ShaderClass.h"

template<typename ShaderBuffers>
Graphic::Shader::ShaderClass<ShaderBuffers>::ShaderClass(ID ShaderID) : m_ShaderID(ShaderID) {}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::Initialize(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;
	ShaderFileInfo info = ShaderList.at(m_ShaderID);

	// 매개 변수 확인 //
	assert(Device && hwnd && info.vsFileName != _T("") && info.psFileName != _T("") && info.vsEntryPoint != "" && info.psEntryPoint != "");

	// shader 초기화 //
	result = InitializeShader(hwnd, Device, VertexDataSemantics);

	return result;
}

template<typename ShaderBuffers>
void Graphic::Shader::ShaderClass<ShaderBuffers>::BeginRender(ID3D11DeviceContext* DeviceContext, const ShaderBuffers& ShaderBufferDatas)
{
	// vertex shader, pixel shader, input layout 바인딩 //
	BindShaderAndInputLayout(DeviceContext);

	// shader에서 사용하는 buffer 설정 //
	SetShaderBuffers(DeviceContext, ShaderBufferDatas);
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
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::InitializeShader(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;
	ShaderFileInfo info = ShaderList.at(m_ShaderID);
	Microsoft::WRL::ComPtr<ID3D10Blob> ErrorMessage = nullptr;				// shader compile 에러메세지
	Microsoft::WRL::ComPtr<ID3D10Blob> VertexShaderBuffer = nullptr;		// vertex shader buffer
	Microsoft::WRL::ComPtr<ID3D10Blob> PixelShaderBuffer = nullptr;			// pixel shader buffer

	// vertex shader code 컴파일 //
	result = D3DCompileFromFile(info.vsFileName.c_str(), NULL, NULL, info.vsEntryPoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &VertexShaderBuffer, &ErrorMessage);
	if (FAILED(result))
	{
		assert(ErrorMessage);
		OutputShaderErrorMessage(hwnd, ErrorMessage.Get(), info.vsFileName);

		return result;
	}

	// pixel shader code 컴파일 //
	result = D3DCompileFromFile(info.psFileName.c_str(), NULL, NULL, info.psEntryPoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &PixelShaderBuffer, &ErrorMessage);
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

	// shader에서 사용하는 buffer들 생성
	result = CreateBuffers(Device);

	// texture sampler state 생성 //
	result = CreateTextureSamplerState(Device);

	return result;
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;
	std::vector<D3D11_INPUT_ELEMENT_DESC> LayoutDesc;
	D3D11_INPUT_ELEMENT_DESC desc = { 0, };

	// vertex input layout 설정
	// vertex input layout 설정는 ModelClass의 VertexType 구조, vertex shader 내부의 VertexInputType 모두 일치해야 함
	for (int i = 0; i < VertexDataSemantics.size(); ++i)
	{
		desc.SemanticName = VertexDataSemantics[i].c_str();
		desc.SemanticIndex = 0;

		if(VertexDataSemantics[i] == "TEXCOORD")
			desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		else
			desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		desc.InputSlot = 0;
		desc.AlignedByteOffset = (i ? D3D11_APPEND_ALIGNED_ELEMENT : 0);
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;

		LayoutDesc.push_back(desc);
	}

	// instance layout 설정
	// instance layout은 world matrix만 있으므로 따로 처리
	for (int i = 0; i < 4; ++i)
	{
		desc.SemanticName = "INSTANCE_WORLD_COLUMN0";
		desc.SemanticIndex = i;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.InputSlot = 1;
		desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		desc.InstanceDataStepRate = 1;

		LayoutDesc.push_back(desc);
	}

	// input layout 생성
	result = Device->CreateInputLayout(LayoutDesc.data(), static_cast<UINT>(LayoutDesc.size()), VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), m_Layout.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::CreateTextureSamplerState(ID3D11Device* Device)
{
	HRESULT result = S_OK;
	D3D11_SAMPLER_DESC SamplerDesc;								// texture sampler state 설정 정보

	// 구조체 초기화 //
	memset(&SamplerDesc, 0, sizeof(SamplerDesc));

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
	MessageBox(hwnd, reinterpret_cast<const wchar_t*>(ErrorMessage->GetBufferPointer()), ShaderFileName.c_str(), MB_OK);
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
	DeviceContext->PSSetSamplers(0, 1, &m_SampleState);
}

template<typename ShaderBuffers>
HRESULT Graphic::Shader::ShaderClass<ShaderBuffers>::CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize)
{
	HRESULT result = S_OK;
	D3D11_BUFFER_DESC ConstantBufferDesc;							// 상수 버퍼 정보

	// 구조체 초기화 //
	memset(&ConstantBufferDesc, 0, sizeof(ConstantBufferDesc));

	// 상수 버퍼 생성 //
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
