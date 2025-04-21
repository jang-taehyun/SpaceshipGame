#include "pch.h"
#include "SpaceshipShaderClass.h"

static ErrorContent e;

SpaceshipShaderClass::SpaceshipShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) : ShaderClass(hwnd, Device, info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SpaceshipShaderClass constructor");

	// input layout 생성 //
	result = CreateInputLayout(Device);
	if (FAILED(result))
	{
		ShutdownShaderBuffer();
		ShutdownLayout();
		throw e;
	}

	// vertex shader buffer, pixel shader buffer 해제 //
	ShutdownShaderBuffer();
}

SpaceshipShaderClass::~SpaceshipShaderClass()
{
	ShutdownLayout();
}

HRESULT SpaceshipShaderClass::CreateInputLayout(ID3D11Device* const& Device)
{
	HRESULT result = S_OK;
	D3D11_INPUT_ELEMENT_DESC PolygonLayout[3];
	UINT ElementsCount = 0;

	// 에러 메세지, 구조체 초기화
	e.title = _T("SpaceshipShaderClass CreateInputLayout()");
	memset(PolygonLayout, 0, sizeof(PolygonLayout));

	// vertex input layout 설정
	// vertex input layout 설정는 ModelClass의 VertexType 구조, vertex shader 내부의 VertexInputType 모두 일치해야 함
	PolygonLayout[0].SemanticName = "POSITION";
	PolygonLayout[0].SemanticIndex = 0;
	PolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[0].InputSlot = 0;
	PolygonLayout[0].AlignedByteOffset = 0;
	PolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[0].InstanceDataStepRate = 0;

	PolygonLayout[1].SemanticName = "TEXCOORD";
	PolygonLayout[1].SemanticIndex = 0;
	PolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	PolygonLayout[1].InputSlot = 0;
	PolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[1].InstanceDataStepRate = 0;

	PolygonLayout[2].SemanticName = "NORMAL";
	PolygonLayout[2].SemanticIndex = 0;
	PolygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	PolygonLayout[2].InputSlot = 0;
	PolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	PolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	PolygonLayout[2].InstanceDataStepRate = 0;

	// input layout의 개수 구하기
	ElementsCount = static_cast<UINT>(sizeof(PolygonLayout) / sizeof(PolygonLayout[0]));

	// input layout 생성
	result = Device->CreateInputLayout(PolygonLayout, ElementsCount, m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), &m_Layout);
	if (FAILED(result))
	{
		e.contents = _T("input layout 생성 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

void SpaceshipShaderClass::ShutdownLayout()
{
	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = nullptr;
	}
}
