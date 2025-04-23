#include "pch.h"
#include "ColorShaderClass.h"
#include "CubeModelClass.h"

static ErrorContent e;

CubeModelClass::CubeModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info, const DirectX::XMFLOAT4& color)
	: ModelClass(hwnd, Device, DeviceContext, info), m_Color(color)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CubeModelClass constructor");

	// shader 초기화 //
	result = InitializeShader(hwnd, Device, info.ShaderInfo);
	if (FAILED(result))
	{
		ReleaseShader();
		throw e;
	}
}

CubeModelClass::~CubeModelClass()
{
	ReleaseShader();
}

HRESULT CubeModelClass::InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CubeModelClass InitializeShader()");

	m_Shader = new ColorShaderClass(hwnd, Device, info);
	if (!m_Shader)
	{
		e.contents = _T("color shader class의 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

HRESULT CubeModelClass::RenderShader(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CubeModelClass RenderShader()");

	result = static_cast<ColorShaderClass*>(GetShader())->Render(DeviceContext, GetIndexCount(), transform, light, camera, m_Color);
	if (FAILED(result))
	{
		e.contents = _T("color shader class의 인스턴스에서 Render 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

void CubeModelClass::ReleaseShader()
{
	if (m_Shader)
	{
		delete m_Shader;
		m_Shader = nullptr;
	}
}

void CubeModelClass::SetBuffers(ID3D11DeviceContext* const& DeviceContext)
{
	// offset(오프셋)과 정점 데이터의 stride(단위) 설정 //
	UINT stride = static_cast<UINT>(sizeof(VertexType));
	UINT offset = 0;

	// input assembler에서 vertex buffer, index buffer 활성화 //
	DeviceContext->IASetVertexBuffers(0, 1, &(GetVertexBuffer()), &stride, &offset);
	DeviceContext->IASetIndexBuffer(GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer에서 그릴 object의 기본 도형 설정 //
	DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
}
