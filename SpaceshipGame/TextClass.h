#pragma once

#include <SpriteFont.h>
#include "ColorClass.h"

class FontShaderClass;
class Position2DClass;

class TextClass
{
public:
	TextClass(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);
	virtual ~TextClass();

	void Render(ID3D11DeviceContext* const& DeviceContext, const std::wstring& text, const DirectX::XMFLOAT2& pos, const DirectX::XMVECTOR& color);

private:
	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);
	void Shutdown();

private:
	static bool IsInitialize;

	std::unique_ptr<DirectX::SpriteFont> m_Font;
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

public:
	TextClass() = delete;
	TextClass(const TextClass& other) = delete;
};