#include "pch.h"
#include "ColorShaderClass.h"

static ErrorContent e;

ColorShaderClass::ColorShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) : ShaderClass(hwnd, Device, info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ColorShaderClass constructor");

	// color 상수 버퍼 생성 //
	result = CreateConstantBuffer(Device, m_ColorBuffer, sizeof(ColorBufferType));
	if (FAILED(result))
	{
		ShutdownBuffer();
		throw e;
	}
}

ColorShaderClass::~ColorShaderClass()
{
	ShutdownBuffer();
}

HRESULT ColorShaderClass::Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const DirectX::XMFLOAT4 color)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ColorShaderClass Render()");

	// shader의 전역변수(buffer) 설정 //
	result = SetShaderParameters(DeviceContext, transform, light, camera, color);
	if (FAILED(result))
		return result;

	// 렌더링 //
	RenderShader(DeviceContext, IndexCount);

	return result;
}

HRESULT ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const DirectX::XMFLOAT4 color)
{
	HRESULT result = S_OK;
	unsigned int SlotNum = 0;									// slot 번호

	// 에러 메세지 초기화 //
	e.title = _T("ColorShaderClass SetShaderParameters()");

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

	// color constant buffer의 내용 업데이트 //
	// pixel shader에서 color constant buffer의 위치 : 1번
	SlotNum = 1;
	result = UpdateColorBuffer(DeviceContext, SlotNum, color);
	if (FAILED(result))
	{
		e.contents = _T("color constant buffer의 내용 업데이트 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

void ColorShaderClass::ShutdownBuffer()
{
	OutputDebugString(_T("ColorShaderClass ShutdownBuffer()"));

	if (m_ColorBuffer)
	{
		m_ColorBuffer->Release();
		m_ColorBuffer = nullptr;
	}
}

HRESULT ColorShaderClass::UpdateColorBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT4& color)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;						// lock
	ColorBufferType* DataPtr = nullptr;								// buffer의 포인터

	// 에러 메세지 초기화 //
	e.title = _T("ColorShaderClass UpdateColorBuffer()");

	// 광원 상수 버퍼의 내용을 CPU가 쓸 수 있도록 잠금
	result = DeviceContext->Map(m_ColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	if (FAILED(result))
	{
		e.contents = _T("lock 실패");
		e.errorCode = result;
		return result;
	}

	// 광원 상수 버퍼의 데이터에 대한 포인터를 가져온다.
	DataPtr = (ColorBufferType*)MappedResource.pData;

	// 광원 상수 버퍼에 데이터(행렬) 복사
	DataPtr->color = color;

	// 광원 상수 버퍼의 잠금을 푼다.
	DeviceContext->Unmap(m_ColorBuffer, 0);

	// pixel shader의 광원 상수 버퍼의 위치 설정 및 color constant buffer의 내용 업데이트
	DeviceContext->PSSetConstantBuffers(slot, 1, &m_ColorBuffer);

	return result;
}
