#include "pch.h"
#include "SpaceshipShaderClass.h"

static ErrorContent e;

SpaceshipShaderClass::SpaceshipShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) : ShaderClass(hwnd, Device, info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("SpaceshipShaderClass constructor");
}

SpaceshipShaderClass::~SpaceshipShaderClass()
{
	
}
