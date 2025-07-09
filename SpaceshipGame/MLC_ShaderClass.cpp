#include "pch.h"
#include "MLC_ShaderClass.h"

Graphic::Shader::MLC_ShaderClass::MLC_ShaderClass(ID ShaderID) : ShaderClass<MLC_ShaderBuffers>(ShaderID) {}

Graphic::Shader::MLC_ShaderClass::MLC_ShaderClass(const MLC_ShaderClass& other) : ShaderClass<MLC_ShaderBuffers>(other)
{
	other.m_MatrixBuffer.CopyTo(m_MatrixBuffer.GetAddressOf());
	other.m_LightBuffer.CopyTo(m_LightBuffer.GetAddressOf());
	other.m_CameraBuffer.CopyTo(m_CameraBuffer.GetAddressOf());
}

Graphic::Shader::MLC_ShaderClass::MLC_ShaderClass(MLC_ShaderClass&& other) noexcept : ShaderClass<MLC_ShaderBuffers>(other)
{
	m_MatrixBuffer = std::move(other.m_MatrixBuffer);
	m_LightBuffer = std::move(other.m_LightBuffer);
	m_CameraBuffer = std::move(other.m_CameraBuffer);
}

Graphic::Shader::MLC_ShaderClass& Graphic::Shader::MLC_ShaderClass::operator=(const MLC_ShaderClass& other)
{
	if (this == &other)
		return *this;

	other.m_MatrixBuffer.CopyTo(m_MatrixBuffer.ReleaseAndGetAddressOf());
	other.m_LightBuffer.CopyTo(m_LightBuffer.ReleaseAndGetAddressOf());
	other.m_CameraBuffer.CopyTo(m_CameraBuffer.ReleaseAndGetAddressOf());

	ShaderClass<MLC_ShaderBuffers>::operator=(other);

	return *this;
}

Graphic::Shader::MLC_ShaderClass& Graphic::Shader::MLC_ShaderClass::operator=(MLC_ShaderClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_MatrixBuffer.Reset();
	m_LightBuffer.Reset();
	m_CameraBuffer.Reset();

	m_MatrixBuffer = std::move(other.m_MatrixBuffer);
	m_LightBuffer = std::move(other.m_LightBuffer);
	m_CameraBuffer = std::move(other.m_CameraBuffer);

	ShaderClass<MLC_ShaderBuffers>::operator=(std::move(other));

	return *this;
}

void Graphic::Shader::MLC_ShaderClass::UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const MLC_ShaderBuffers& ShaderBufferDatas)
{
	HRESULT result = S_OK;

	// matrix constant buffer의 내용 업데이트 //
	result = UpdateMatrixBuffer(DeviceContext, ShaderBufferDatas.transform);

	// light constant buffer의 내용 업데이트 //
	result = UpdateLightBuffer(DeviceContext, ShaderBufferDatas.light);

	// camera constant buffer의 내용 업데이트 //
	result = UpdateCameraBuffer(DeviceContext, ShaderBufferDatas.camera);
}

HRESULT Graphic::Shader::MLC_ShaderClass::CreateBuffers(ID3D11Device* Device)
{
	HRESULT result = S_OK;

	// 행렬 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_MatrixBuffer.GetAddressOf(), sizeof(MatrixBufferType));

	// light 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_LightBuffer.GetAddressOf(), sizeof(LightBufferType));

	// camera 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_CameraBuffer.GetAddressOf(), sizeof(CameraBufferType));

	return result;
}

void Graphic::Shader::MLC_ShaderClass::SetShaderBuffers(ID3D11DeviceContext* DeviceContext)
{
	UINT slot = 0;

	// vertex shader에서 상수 버퍼의 위치 설정
	// vertex shader에서 matrix constant buffer의 위치 : 0번
	slot = 0;
	DeviceContext->VSSetConstantBuffers(slot, 1, m_MatrixBuffer.GetAddressOf());

	// pixel shader의 광원 상수 버퍼의 위치 설정
	// pixel shader에서 light constant buffer의 위치 : 0번
	slot = 0;
	DeviceContext->PSSetConstantBuffers(slot, 1, m_LightBuffer.GetAddressOf());

	// vertex shader에서 상수 버퍼의 위치 설정 및 camera constant buffer의 내용 업데이트
	// vertex shader에서 camera constant buffer의 위치 : 1번
	slot = 1;
	DeviceContext->VSSetConstantBuffers(slot, 1, m_CameraBuffer.GetAddressOf());
}

HRESULT Graphic::Shader::MLC_ShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, const MatrixBufferType& transform)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;			// lock
	MatrixBufferType* DataPtr = nullptr;				// buffer의 포인터
	DirectX::XMMATRIX viewMatrix;						// view
	DirectX::XMMATRIX projectionMatrix;					// projection

	// 행렬들을 HLSL에 맞게 변환 //
	// 행렬들을 transpose 연산하여 shader에서 사용할 수 있도록 한다.
	viewMatrix = DirectX::XMMatrixTranspose(transform.View);
	projectionMatrix = DirectX::XMMatrixTranspose(transform.Projection);

	// matrix constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// matrix constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = static_cast<MatrixBufferType*>(MappedResource.pData);

	// matrix constant buffer에 데이터(행렬) 복사
	DataPtr->View = viewMatrix;
	DataPtr->Projection = projectionMatrix;

	// matrix constant buffer의 잠금을 풀어 GPU에 반영
	DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);

	return result;
}

HRESULT Graphic::Shader::MLC_ShaderClass::UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, const LightBufferType& light)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	LightBufferType* DataPtr = nullptr;								// buffer의 포인터

	// 광원 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	result = DeviceContext->Map(m_LightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// 광원 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	DataPtr = static_cast<LightBufferType*>(MappedResource.pData);

	// 광원 상수 버퍼에 데이터(행렬) 복사
	DataPtr->AmbientColor = light.AmbientColor;
	DataPtr->DiffuseColor = light.DiffuseColor;
	DataPtr->LightDirection = light.LightDirection;
	DataPtr->padding1 = 0.f;
	DataPtr->SpecularColor = light.SpecularColor;
	DataPtr->SpecularPower = light.SpecularPower;
	DataPtr->padding2[0] = { 0.f, };

	// 광원 상수 버퍼의 잠금을 풀어 GPU에 반영
	DeviceContext->Unmap(m_LightBuffer.Get(), 0);

	return result;
}

HRESULT Graphic::Shader::MLC_ShaderClass::UpdateCameraBuffer(ID3D11DeviceContext* DeviceContext, const CameraBufferType& camera)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	CameraBufferType* DataPtr = nullptr;							// buffer의 포인터

	// camera constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_CameraBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// camera constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = static_cast<CameraBufferType*>(MappedResource.pData);

	// camera constant buffer에 데이터(행렬) 복사
	DataPtr->CameraPosition = camera.CameraPosition;
	DataPtr->padding = 0.f;

	// camera constant buffer의 잠금을 풀어 GPU에 반영
	DeviceContext->Unmap(m_CameraBuffer.Get(), 0);

	return result;
}

