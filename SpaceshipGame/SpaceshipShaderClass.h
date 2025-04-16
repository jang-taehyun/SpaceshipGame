#pragma once

#include "ShaderClass.h"

class SpaceshipShaderClass : public ShaderClass
{
private:
	typedef struct _vertex_type_
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	} VertexType;

public:
	SpaceshipShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~SpaceshipShaderClass();

private:
	virtual HRESULT CreateInputLayout(ID3D11Device* const& Device) override;
	virtual void ShutdownLayout() override;

public:
	SpaceshipShaderClass() = delete;
	SpaceshipShaderClass(const SpaceshipShaderClass& other) = delete;
};

