#include "pch.h"
#include "Cube_ShaderClass.h"

Graphic::Shader::Cube_ShaderClass::Cube_ShaderClass(ID ShaderID)
	: ShaderClass<M_ShaderBuffer>(ShaderID)
{}

Graphic::Shader::Cube_ShaderClass::Cube_ShaderClass(const Cube_ShaderClass& other)
	: ShaderClass<M_ShaderBuffer>(other)
{
	other.m_MatrixBuffer.CopyTo(m_MatrixBuffer.GetAddressOf());
}

Graphic::Shader::Cube_ShaderClass::Cube_ShaderClass(Cube_ShaderClass&& other) noexcept
	: ShaderClass<M_ShaderBuffer>(other)
{
	m_MatrixBuffer = std::move(other.m_MatrixBuffer);
}

Graphic::Shader::Cube_ShaderClass& Graphic::Shader::Cube_ShaderClass::operator=(const Cube_ShaderClass& other)
{
	if (this == &other)
		return *this;

	other.m_MatrixBuffer.CopyTo(m_MatrixBuffer.ReleaseAndGetAddressOf());

	ShaderClass<M_ShaderBuffer>::operator=(other);

	return *this;
}

Graphic::Shader::Cube_ShaderClass& Graphic::Shader::Cube_ShaderClass::operator=(Cube_ShaderClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_MatrixBuffer.Reset();
	m_MatrixBuffer = std::move(other.m_MatrixBuffer);

	ShaderClass<M_ShaderBuffer>::operator=(std::move(other));

	return *this;
}

void Graphic::Shader::Cube_ShaderClass::UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const M_ShaderBuffer& ShaderBufferDatas)
{
	HRESULT result = S_OK;

	// matrix constant buffer의 내용 업데이트 //
	result = UpdateMatrixBuffer(DeviceContext, ShaderBufferDatas.transform);
}

HRESULT Graphic::Shader::Cube_ShaderClass::CreateBuffers(ID3D11Device* Device)
{
	HRESULT result = S_OK;

	// 행렬 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_MatrixBuffer.GetAddressOf(), sizeof(MatrixBufferType));

	return result;
}

void Graphic::Shader::Cube_ShaderClass::SetShaderBuffers(ID3D11DeviceContext* DeviceContext)
{
	UINT slot = 0;

	// vertex shader에서 상수 버퍼의 위치 설정
	// vertex shader에서 matrix constant buffer의 위치 : 0번
	slot = 0;
	DeviceContext->VSSetConstantBuffers(slot, 1, m_MatrixBuffer.GetAddressOf());
}

HRESULT Graphic::Shader::Cube_ShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, const MatrixBufferType& transform)
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