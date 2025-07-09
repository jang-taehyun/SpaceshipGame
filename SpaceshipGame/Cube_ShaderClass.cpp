#include "pch.h"
#include "Cube_ShaderClass.h"

Graphic::Shader::Cube_ShaderClass::Cube_ShaderClass(ID ShaderID) : ShaderClass<None_ShaderBuffer>(ShaderID) {}

void Graphic::Shader::Cube_ShaderClass::UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const None_ShaderBuffer& ShaderBufferDatas)
{}

HRESULT Graphic::Shader::Cube_ShaderClass::CreateBuffers(ID3D11Device* Device)
{
	return S_OK;
}

void Graphic::Shader::Cube_ShaderClass::SetShaderBuffers(ID3D11DeviceContext* DeviceContext)
{}