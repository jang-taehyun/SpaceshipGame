#include "pch.h"
#include "LightClass.h"
#include "IObjectClass.h"
#include "CameraClass.h"
#include "TypeConverterClass.h"
#include "ShaderClass.h"

Graphic::Shader::ShaderClass::ShaderClass(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics, ID ShaderID) : m_ShaderID(ShaderID)
{
	HRESULT result = Initialize(hwnd, Device, VertexDataSemantics);
	assert(SUCCEEDED(result));
}

void Graphic::Shader::ShaderClass::BeginRender(ID3D11DeviceContext* DeviceContext, const TransformMatrixData& transform, const LightClass* light, const Object::IObjectClass* camera)
{
	// vertex shader, pixel shader, input layout 바인딩 //
	BindShaderAndInputLayout(DeviceContext);

	// shader에서 사용하는 buffer 설정 //
	SetShaderBuffers(DeviceContext, transform, light, camera);
}

HRESULT Graphic::Shader::ShaderClass::Initialize(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics)
{
	HRESULT result = S_OK;
	ShaderFileInfo info = ShaderList.at(m_ShaderID);

	// 매개 변수 확인 //
	assert(Device && hwnd && info.vsFileName != _T("") && info.psFileName != _T("") && info.vsEntryPoint != "" && info.psEntryPoint != "");

	// shader 초기화 //
	result = InitializeShader(hwnd, Device, VertexDataSemantics);

	return result;
}

HRESULT Graphic::Shader::ShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& Material)
{
	HRESULT result = S_OK;

	// pixel shader에서 사용할 shader texture resource(Texture2D) 설정 //
	// GPU 파이프라인에 텍스처 데이터를 바인드
	for(int i=0; i<Material.size(); ++i)
		DeviceContext->PSSetShaderResources(i, 1, Material[i].GetAddressOf());

	// 렌더링 //
	DeviceContext->DrawIndexed(IndexCount, 0, 0);

	return result;
}

HRESULT Graphic::Shader::ShaderClass::InitializeShader(HWND hwnd, ID3D11Device* Device, const std::vector<std::string>& VertexDataSemantics)
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

	// 행렬 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_MatrixBuffer.GetAddressOf(), sizeof(MatrixBufferType));

	// light 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_LightBuffer.GetAddressOf(), sizeof(LightBufferType));

	// camera 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_CameraBuffer.GetAddressOf(), sizeof(CameraBufferType));

	// texture sampler state 생성 //
	result = CreateTextureSamplerState(Device);

	return result;
}

HRESULT Graphic::Shader::ShaderClass::CreateInputLayout(ID3D11Device* Device, ID3D10Blob* VertexShaderBuffer, const std::vector<std::string>& VertexDataSemantics)
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

	// input layout 생성
	result = Device->CreateInputLayout(LayoutDesc.data(), static_cast<UINT>(LayoutDesc.size()), VertexShaderBuffer->GetBufferPointer(), VertexShaderBuffer->GetBufferSize(), m_Layout.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

HRESULT Graphic::Shader::ShaderClass::CreateConstantBuffer(ID3D11Device* Device, ID3D11Buffer** Buffer, UINT BufferSize)
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

HRESULT Graphic::Shader::ShaderClass::CreateTextureSamplerState(ID3D11Device* Device)
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

void Graphic::Shader::ShaderClass::OutputShaderErrorMessage(HWND hwnd, ID3D10Blob* ErrorMessage, const std::wstring& ShaderFileName)
{
	MessageBox(hwnd, reinterpret_cast<const wchar_t*>(ErrorMessage->GetBufferPointer()), ShaderFileName.c_str(), MB_OK);
}

void Graphic::Shader::ShaderClass::SetShaderBuffers(ID3D11DeviceContext* DeviceContext, const TransformMatrixData& transform, const LightClass* light, const Object::IObjectClass* camera)
{
	HRESULT result = S_OK;
	UINT SlotNum = 0;												// slot 번호

	// matrix constant buffer의 내용 업데이트 //
	// vertex shader에서 matrix constant buffer의 위치 : 0번
	SlotNum = 0;
	result = UpdateMatrixBuffer(DeviceContext, SlotNum, transform);

	// light constant buffer의 내용 업데이트 //
	// pixel shader에서 light constant buffer의 위치 : 0번
	SlotNum = 0;
	result = UpdateLightBuffer(DeviceContext, SlotNum, light);

	// camera constant buffer의 내용 업데이트 //
	// vertex shader에서 camera constant buffer의 위치 : 1번
	SlotNum = 1;
	result = UpdateCameraBuffer(DeviceContext, SlotNum, camera);
}

HRESULT Graphic::Shader::ShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const TransformMatrixData& transform)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	MatrixBufferType* DataPtr = nullptr;							// buffer의 포인터
	DirectX::XMMATRIX worldMatrix;									// world
	DirectX::XMMATRIX viewMatrix;									// view
	DirectX::XMMATRIX projectionMatrix;								// projection

	// 행렬들을 HLSL에 맞게 변환 //
	// 행렬들을 transpose 연산하여 shader에서 사용할 수 있도록 한다.
	worldMatrix = DirectX::XMMatrixTranspose(transform.world);
	viewMatrix = DirectX::XMMatrixTranspose(transform.view);
	projectionMatrix = DirectX::XMMatrixTranspose(transform.projection);

	// matrix constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// matrix constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = (MatrixBufferType*)MappedResource.pData;

	// matrix constant buffer에 데이터(행렬) 복사
	DataPtr->World = worldMatrix;
	DataPtr->View = viewMatrix;
	DataPtr->Projection = projectionMatrix;

	// matrix constant buffer의 잠금을 푼다.
	DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 matrix constant buffer의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, m_MatrixBuffer.GetAddressOf());

	return result;
}

HRESULT Graphic::Shader::ShaderClass::UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const LightClass* light)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	LightBufferType* DataPtr = nullptr;								// buffer의 포인터

	// 광원 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	result = DeviceContext->Map(m_LightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// 광원 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	DataPtr = (LightBufferType*)MappedResource.pData;

	// 광원 상수 버퍼에 데이터(행렬) 복사
	DataPtr->AmbientColor = light->GetAmbientColor();
	DataPtr->DiffuseColor = light->GetDiffuseColor();
	DataPtr->LightDirection = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(light->GetDirection());
	DataPtr->padding1 = 0.f;
	DataPtr->SpecularColor = light->GetSpecularColor();
	DataPtr->SpecularPower = light->GetSpecularPower();
	DataPtr->padding2[0] = { 0.f, };

	// 광원 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_LightBuffer.Get(), 0);

	// pixel shader의 광원 상수 버퍼의 위치 설정 및 light constant buffer의 내용 업데이트
	DeviceContext->PSSetConstantBuffers(slot, 1, m_LightBuffer.GetAddressOf());

	return result;
}

HRESULT Graphic::Shader::ShaderClass::UpdateCameraBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const Object::IObjectClass* camera)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	CameraBufferType* DataPtr = nullptr;							// buffer의 포인터

	// camera constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_CameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// camera constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = (CameraBufferType*)MappedResource.pData;

	// camera constant buffer에 데이터(행렬) 복사
	DataPtr->CameraPosition = Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(static_cast<const Object::GameObjectClass*>(camera)->GetPosition());
	DataPtr->padding = 0.f;

	// camera constant buffer의 잠금을 푼다.
	DeviceContext->Unmap(m_CameraBuffer.Get(), 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 camera constant buffer의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, m_CameraBuffer.GetAddressOf());

	return result;
}

void Graphic::Shader::ShaderClass::BindShaderAndInputLayout(ID3D11DeviceContext* DeviceContext)
{
	// vertex input layout 설정 //
	DeviceContext->IASetInputLayout(m_Layout.Get());

	// vertex shader와 pixel shader 설정 //
	DeviceContext->VSSetShader(m_VertexShader.Get(), NULL, 0);
	DeviceContext->PSSetShader(m_PixelShader.Get(), NULL, 0);

	// pixel shader에서 사용할 sampler state 설정(SamplerState) //
	DeviceContext->PSSetSamplers(0, 1, &m_SampleState);
}