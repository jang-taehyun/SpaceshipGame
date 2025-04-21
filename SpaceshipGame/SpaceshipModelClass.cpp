#include "pch.h"
#include "SpaceshipShaderClass.h"
#include "SpaceshipModelClass.h"

static ErrorContent e;

SpaceshipModelClass::SpaceshipModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info) : ModelClass(hwnd, Device, DeviceContext, info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SpaceshipModelClass constructor");

	// shader 초기화 //
	result = InitializeShader(hwnd, Device, info.ShaderInfo);
	if (FAILED(result))
	{
		ReleaseShader();
		throw e;
	}
}

SpaceshipModelClass::~SpaceshipModelClass()
{
	ReleaseShader();
}

HRESULT SpaceshipModelClass::InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SpaceshipModelClass InitializeShader()");

	m_Shader = new SpaceshipShaderClass(hwnd, Device, info);
	if (!m_Shader)
	{
		e.contents = _T("spaceship shader class의 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

HRESULT SpaceshipModelClass::RenderShader(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SpaceshipModelClass RenderShader()");

	result = static_cast<SpaceshipShaderClass*>(GetShader())->Render(DeviceContext, GetIndexCount(), transform, light, camera, GetTextureArray());
	if (FAILED(result))
	{
		e.contents = _T("spaceship shader class의 인스턴스에서 Render 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

void SpaceshipModelClass::ReleaseShader()
{
	if (m_Shader)
	{
		delete m_Shader;
		m_Shader = nullptr;
	}
}
