#pragma once

#include "ModelClass.h"

class SpaceshipModelClass : public ModelClass
{
public:
	explicit SpaceshipModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& ModelInfo);
	virtual ~SpaceshipModelClass();
	
private:
	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info) override;
	virtual HRESULT RenderShader(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera) override;
	virtual void ReleaseShader() override;

public:
	SpaceshipModelClass() = delete;
	SpaceshipModelClass(const SpaceshipModelClass& other) = delete;
};