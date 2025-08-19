#include "pch.h"
#include "TerrainShaderClass.h"

Graphic::Shader::TerrainShaderClass::TerrainShaderClass(ID ShaderID) : ShaderClass<ML_ShaderBuffers>(ShaderID) {}

Graphic::Shader::TerrainShaderClass::TerrainShaderClass(const TerrainShaderClass& other) : ShaderClass<ML_ShaderBuffers>(other)
{
	other.m_MatrixBuffer.CopyTo(m_MatrixBuffer.GetAddressOf());
	other.m_LightBuffer.CopyTo(m_LightBuffer.GetAddressOf());
}

Graphic::Shader::TerrainShaderClass::TerrainShaderClass(TerrainShaderClass&& other) noexcept : ShaderClass<ML_ShaderBuffers>(other)
{
	m_MatrixBuffer = std::move(other.m_MatrixBuffer);
	m_LightBuffer = std::move(other.m_LightBuffer);
}

Graphic::Shader::TerrainShaderClass& Graphic::Shader::TerrainShaderClass::operator=(const TerrainShaderClass& other)
{
	if (this == &other)
		return *this;

	other.m_MatrixBuffer.CopyTo(m_MatrixBuffer.ReleaseAndGetAddressOf());
	other.m_LightBuffer.CopyTo(m_LightBuffer.ReleaseAndGetAddressOf());

	ShaderClass<ML_ShaderBuffers>::operator=(other);

	return *this;
}

Graphic::Shader::TerrainShaderClass& Graphic::Shader::TerrainShaderClass::operator=(TerrainShaderClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_MatrixBuffer.Reset();
	m_LightBuffer.Reset();

	m_MatrixBuffer = std::move(other.m_MatrixBuffer);
	m_LightBuffer = std::move(other.m_LightBuffer);

	ShaderClass<ML_ShaderBuffers>::operator=(std::move(other));

	return *this;
}

void Graphic::Shader::TerrainShaderClass::UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const ML_ShaderBuffers& ShaderBufferDatas)
{
	HRESULT result = S_OK;

	// matrix constant buffer의 내용 업데이트 //
	result = UpdateMatrixBuffer(DeviceContext, ShaderBufferDatas.transform);

	// light constant buffer의 내용 업데이트 //
	result = UpdateLightBuffer(DeviceContext, ShaderBufferDatas.light);
}

void Graphic::Shader::TerrainShaderClass::Render(ID3D11DeviceContext* DeviceContext, int IndexCount, ID3D11ShaderResourceView* texture)
{
	// pixel shader에서 사용할 shader texture resource(Texture2D) 설정 //
	// GPU 파이프라인에 텍스처 데이터를 바인드
	DeviceContext->PSSetShaderResources(0, 1, &texture);

	// 렌더링 //
	DeviceContext->DrawIndexed(IndexCount, 0, 0);
}

HRESULT Graphic::Shader::TerrainShaderClass::CreateBuffers(ID3D11Device* Device)
{
	HRESULT result = S_OK;

	// 행렬 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_MatrixBuffer.GetAddressOf(), sizeof(MatrixBufferType));

	// light 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_LightBuffer.GetAddressOf(), sizeof(LightBufferType));

	return result;
}

void Graphic::Shader::TerrainShaderClass::SetShaderBuffers(ID3D11DeviceContext* DeviceContext)
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
}

HRESULT Graphic::Shader::TerrainShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, const MatrixBufferType& transform)
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

HRESULT Graphic::Shader::TerrainShaderClass::UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, const LightBufferType& light)
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