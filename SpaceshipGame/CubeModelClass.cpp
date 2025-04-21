#include "pch.h"
#include "MultiTextureShaderClass.h"
#include "CubeModelClass.h"

static ErrorContent e;

CubeModelClass::CubeModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info) : ModelClass(hwnd, Device, DeviceContext, info)
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

	m_Shader = new MultiTextureShaderClass(hwnd, Device, info);
	if (!m_Shader)
	{
		e.contents = _T("multi-texture shader class의 인스턴스 생성 실패");
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

	result = static_cast<MultiTextureShaderClass*>(GetShader())->Render(DeviceContext, GetIndexCount(), transform, light, camera, GetTextureArray());
	if (FAILED(result))
	{
		e.contents = _T("multi-texture shader class의 인스턴스에서 Render 실패");
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
