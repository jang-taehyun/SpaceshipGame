#include "pch.h"
#include "MLC_ShaderClass.h"

Graphic::Shader::MLC_ShaderClass::MLC_ShaderClass(ID ShaderID) : ShaderClass<MLC_ShaderBuffers>(ShaderID) {}

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

void Graphic::Shader::MLC_ShaderClass::SetShaderBuffers(ID3D11DeviceContext* DeviceContext, const Graphic::Shader::MLC_ShaderBuffers& ShaderBufferDatas)
{
	HRESULT result = S_OK;
	UINT SlotNum = 0;												// slot 번호

	// matrix constant buffer의 내용 업데이트 //
	// vertex shader에서 matrix constant buffer의 위치 : 0번
	SlotNum = 0;
	result = UpdateMatrixBuffer(DeviceContext, SlotNum, ShaderBufferDatas.transform);

	// light constant buffer의 내용 업데이트 //
	// pixel shader에서 light constant buffer의 위치 : 0번
	SlotNum = 0;
	result = UpdateLightBuffer(DeviceContext, SlotNum, ShaderBufferDatas.light);

	// camera constant buffer의 내용 업데이트 //
	// vertex shader에서 camera constant buffer의 위치 : 1번
	SlotNum = 1;
	result = UpdateCameraBuffer(DeviceContext, SlotNum, ShaderBufferDatas.camera);
}

HRESULT Graphic::Shader::MLC_ShaderClass::UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const MatrixBufferType& transform)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;			// lock
	MatrixBufferType* DataPtr = nullptr;				// buffer의 포인터
	DirectX::XMMATRIX worldMatrix;						// world
	DirectX::XMMATRIX viewMatrix;						// view
	DirectX::XMMATRIX projectionMatrix;					// projection

	// 행렬들을 HLSL에 맞게 변환 //
	// 행렬들을 transpose 연산하여 shader에서 사용할 수 있도록 한다.
	worldMatrix = DirectX::XMMatrixTranspose(transform.World);
	viewMatrix = DirectX::XMMatrixTranspose(transform.View);
	projectionMatrix = DirectX::XMMatrixTranspose(transform.Projection);

	// matrix constant buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_MatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// matrix constant buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = static_cast<MatrixBufferType*>(MappedResource.pData);

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

HRESULT Graphic::Shader::MLC_ShaderClass::UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const LightBufferType& light)
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

	// 광원 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_LightBuffer.Get(), 0);

	// pixel shader의 광원 상수 버퍼의 위치 설정 및 light constant buffer의 내용 업데이트
	DeviceContext->PSSetConstantBuffers(slot, 1, m_LightBuffer.GetAddressOf());

	return result;
}

HRESULT Graphic::Shader::MLC_ShaderClass::UpdateCameraBuffer(ID3D11DeviceContext* DeviceContext, UINT slot, const CameraBufferType& camera)
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

	// camera constant buffer의 잠금을 푼다.
	DeviceContext->Unmap(m_CameraBuffer.Get(), 0);

	// vertex shader에서 상수 버퍼의 위치 설정 및 camera constant buffer의 내용 업데이트
	DeviceContext->VSSetConstantBuffers(slot, 1, m_CameraBuffer.GetAddressOf());

	return result;
}

