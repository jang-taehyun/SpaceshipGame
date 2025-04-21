#include "pch.h"
#include "LightClass.h"
#include "CameraClass.h"
#include "AffineClass.h"
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

HRESULT ShaderClass::Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const std::vector<ID3D11ShaderResourceView*>& Textures)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass Render()");

	// shader의 전역변수(buffer) 설정 //
	result = SetShaderParameters(DeviceContext, transform, light, camera, Textures);
	if (FAILED(result))
		return result;

	// 렌더링 //
	RenderShader(DeviceContext, IndexCount);

	return result;
}

HRESULT ShaderClass::InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
	HRESULT result = S_OK;
	ID3D10Blob* ErrorMessage = nullptr;					// shader compile 에러메세지

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass InitializeShader()");

	// vertex shader code 컴파일 //
	result = D3DCompileFromFile(info.vsFileName.c_str(), NULL, NULL, info.vsEntryPoint.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_VertexShaderBuffer, &ErrorMessage);
	if (FAILED(result))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.vsFileName);
		}
		else
		{
			e.contents = _T("vertex shader file이 없습니다.");
			e.errorCode = result;
		}

		return result;
	}

	// pixel shader code 컴파일 //
	result = D3DCompileFromFile(info.psFileName.c_str(), NULL, NULL, info.psEntryPoint.c_str(), "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_PixelShaderBuffer, &ErrorMessage);
	if (FAILED(result))
	{
		if (ErrorMessage)
		{
			OutputShaderErrorMessage(ErrorMessage, hwnd, info.psFileName);
		}
		else
		{
			e.contents = _T("pixel shader file이 없습니다.");
			e.errorCode = result;
		}

		return result;
	}

	// vertex shader 생성 //
	result = Device->CreateVertexShader(m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(result))
	{
		e.contents = _T("vertex shader 생성 실패");
		e.errorCode = result;
		return result;
	}

	// pixel shader 생성 //
	result = Device->CreatePixelShader(m_PixelShaderBuffer->GetBufferPointer(), m_PixelShaderBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(result))
	{
		e.contents = _T("vertex shader 생성 실패");
		e.errorCode = result;
		return result;
	}

	// 행렬 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_MatrixBuffer, sizeof(MatrixBufferType));
	if (FAILED(result))
		return result;

	// light 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_LightBuffer, sizeof(LightBufferType));
	if (FAILED(result))
		return result;

	// camera 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_CameraBuffer, sizeof(CameraBufferType));
	if (FAILED(result))
		return result;

	// texture sampler state 생성 //
	result = CreateTextureSamplerState(Device, m_SampleState);
	if (FAILED(result))
		return result;

	return result;
}

HRESULT ShaderClass::CreateConstantBuffer(ID3D11Device* const& Device, ID3D11Buffer*& Buffer, const UINT& BufferSize)
{
	HRESULT result = S_OK;
	D3D11_BUFFER_DESC ConstantBufferDesc;							// 상수 버퍼 정보

	// 에러 메세지, 구조체 초기화 //
	e.title = _T("ShaderClass CreateConstantBuffer()");
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
	result = Device->CreateBuffer(&ConstantBufferDesc, NULL, &Buffer);
	if (FAILED(result))
	{
		e.contents = _T("상수 버퍼 생성 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

HRESULT ShaderClass::CreateTextureSamplerState(ID3D11Device* const& Device, ID3D11SamplerState*& sampler)
{
	HRESULT result = S_OK;
	D3D11_SAMPLER_DESC SamplerDesc;								// texture sampler state 설정 정보

	// 에러 메세지, 구조체 초기화 //
	e.title = _T("ShaderClass CreateTextureSamplerState()");
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
	result = Device->CreateSamplerState(&SamplerDesc, &sampler);
	if (FAILED(result))
	{
		e.contents = _T("texture sampler state 생성 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

void ShaderClass::ShutdownShader()
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

HRESULT ShaderClass::SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const std::vector<ID3D11ShaderResourceView*>& Textures)
{
	HRESULT result = S_OK;
	unsigned int SlotNum = 0;									// slot 번호

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass SetShaderParameters()");

	// matrix constant buffer의 내용 업데이트 //
	// vertex shader에서 matrix constant buffer의 위치 : 0번
	SlotNum = 0;
	result = UpdateMatrixBuffer(DeviceContext, SlotNum, transform);
	if (FAILED(result))
	{
		e.contents = _T("matrix constant buffer의 내용 업데이트 실패");
		e.errorCode = result;
		return result;
	}

	// light constant buffer의 내용 업데이트 //
	// pixel shader에서 light constant buffer의 위치 : 0번
	SlotNum = 0;
	result = UpdateLightBuffer(DeviceContext, SlotNum, light);
	if (FAILED(result))
	{
		e.contents = _T("light constant buffer의 내용 업데이트 실패");
		e.errorCode = result;
		return result;
	}

	// camera constant buffer의 내용 업데이트 //
	// vertex shader에서 camera constant buffer의 위치 : 1번
	SlotNum = 1;
	result = UpdateCameraBuffer(DeviceContext, SlotNum, camera);
	if (FAILED(result))
	{
		e.contents = _T("camera constant buffer의 내용 업데이트 실패");
		e.errorCode = result;
		return result;
	}

	// pixel shader에서 사용할 shader texture resource(Texture2D) 설정 //
	// GPU 파이프라인에 텍스처 데이터를 바인드
	DeviceContext->PSSetShaderResources(0, (UINT)Textures.size(), Textures.data());

	return result;
}

HRESULT ShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const TransformMatrixData& transform)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	MatrixBufferType* DataPtr = nullptr;							// buffer의 포인터
	DirectX::XMMATRIX worldMatrix;									// world
	DirectX::XMMATRIX viewMatrix;									// view
	DirectX::XMMATRIX projectionMatrix;								// projection

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass UpdateMatrixBuffer()");

	// 행렬들을 HLSL에 맞게 변환 //
	// 행렬들을 transpose 연산하여 shader에서 사용할 수 있도록 한다.
	worldMatrix = DirectX::XMMatrixTranspose(transform.world);
	viewMatrix = DirectX::XMMatrixTranspose(transform.view);
	projectionMatrix = DirectX::XMMatrixTranspose(transform.projection);

	// matrix constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(result))
	{
		e.contents = _T("lock 실패");
		e.errorCode = result;
		return result;
	}

	// matrix constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = (MatrixBufferType*)MappedResource.pData;

	// matrix constant buffer에 데이터(행렬) 복사
	DataPtr->World = worldMatrix;
	DataPtr->View = viewMatrix;
	DataPtr->Projection = projectionMatrix;

	// matrix constant buffer의 잠금을 푼다.
	DeviceContext->Unmap(m_MatrixBuffer, 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 matrix constant buffer의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, &m_MatrixBuffer);

	return result;
}

HRESULT ShaderClass::UpdateLightBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const LightClass* const& light)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	LightBufferType* DataPtr = nullptr;								// buffer의 포인터

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass UpdateLightBuffer()");

	// 광원 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	result = DeviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(result))
	{
		e.contents = _T("lock 실패");
		e.errorCode = result;
		return result;
	}

	// 광원 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	DataPtr = (LightBufferType*)MappedResource.pData;

	// 광원 상수 버퍼에 데이터(행렬) 복사
	DataPtr->AmbientColor = light->GetAmbientColor();
	DataPtr->DiffuseColor = light->GetDiffuseColor();
	DataPtr->LightDirection = light->GetDirection();
	DataPtr->SpecularColor = light->GetSpecularColor();
	DataPtr->SpecularPower = light->GetSpecularPower();

	// 광원 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_LightBuffer, 0);

	// pixel shader의 광원 상수 버퍼의 위치 설정 및 light constant buffer의 내용 업데이트
	DeviceContext->PSSetConstantBuffers(slot, 1, &m_LightBuffer);

	return result;
}

HRESULT ShaderClass::UpdateCameraBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const CameraClass* const& camera)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	CameraBufferType* DataPtr = nullptr;							// buffer의 포인터

	// 에러 메세지 초기화 //
	e.title = _T("ShaderClass UpdateCameraBuffer()");

	// camera constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_CameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(result))
	{
		e.contents = _T("lock 실패");
		e.errorCode = result;
		return result;
	}

	// camera constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = (CameraBufferType*)MappedResource.pData;

	// camera constant buffer에 데이터(행렬) 복사
	DataPtr->CameraPosition = DirectX::XMFLOAT3(camera->GetTransformObject()->GetPosition().x, camera->GetTransformObject()->GetPosition().y, camera->GetTransformObject()->GetPosition().z);
	DataPtr->padding = 0.f;

	// camera constant buffer의 잠금을 푼다.
	DeviceContext->Unmap(m_MatrixBuffer, 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 camera constant buffer의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, &m_CameraBuffer);

	return result;
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
