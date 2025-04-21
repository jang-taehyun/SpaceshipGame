#pragma once

#include "ShaderClass.h"

class SpaceshipShaderClass : public ShaderClass
{
public:
	explicit SpaceshipShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~SpaceshipShaderClass();

public:
	SpaceshipShaderClass() = delete;
	SpaceshipShaderClass(const SpaceshipShaderClass& other) = delete;
};

