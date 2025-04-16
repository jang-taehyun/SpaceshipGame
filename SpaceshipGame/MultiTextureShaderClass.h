#pragma once

#include "ShaderClass.h"

class MultiTextureShaderClass : public ShaderClass
{
private:
	typedef struct _vertex_type_
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	} VertexType;

public:
	MultiTextureShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~MultiTextureShaderClass();

private:
	virtual HRESULT CreateInputLayout(ID3D11Device* const& Device) override;
	virtual void ShutdownLayout() override;

public:
	MultiTextureShaderClass() = delete;
	MultiTextureShaderClass(const MultiTextureShaderClass& other) = delete;
};

