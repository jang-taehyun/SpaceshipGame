#pragma once

#include <SpriteFont.h>

class D3DClass;

class TextRenderClass
{
public:
	explicit TextRenderClass(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);
	virtual ~TextRenderClass();

	void Render(D3DClass* const& DirectX3D, const std::wstring& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color);

private:
	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);
	void Shutdown();

	void BeginRender(D3DClass* const& DirectX3D);
	void Rendering(const std::wstring& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color);
	void EndRender(D3DClass* const& DirectX3D);

private:
	static bool IsInitialize;

	std::unique_ptr<DirectX::SpriteFont> m_Font;
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

public:
	TextRenderClass() = delete;
	TextRenderClass(const TextRenderClass& other) = delete;
};