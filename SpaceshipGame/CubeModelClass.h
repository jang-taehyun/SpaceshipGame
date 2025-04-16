#pragma once

#include "ModelClass.h"

class CubeModelClass : public ModelClass
{
public:
	CubeModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info);
	virtual ~CubeModelClass();

private:
	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) override;
	virtual HRESULT RenderShader(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform) override;
	virtual void ReleaseShader() override;

public:
	CubeModelClass() = delete;
	CubeModelClass(const CubeModelClass& other) = delete;
};

