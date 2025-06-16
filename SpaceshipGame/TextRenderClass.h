#pragma once

#include <SpriteFont.h>

class D3DClass;
class TextManagerClass;
class ITextClass;

class TextRenderClass
{
public:
	TextRenderClass(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext);
	virtual ~TextRenderClass();

	void Render(const D3DClass* DirectX3D, const TextManagerClass* TextManager);

private:
	HRESULT Initialize(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext);

	void BeginRender(const D3DClass* DirectX3D);
	void Rendering(const ITextClass* Text);
	void EndRender(const D3DClass* DirectX3D);

private:
	static bool IsInitialize;

	std::unique_ptr<DirectX::SpriteFont> m_Font;
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

public:
	TextRenderClass(const TextRenderClass& other) = delete;
	TextRenderClass(TextRenderClass&& other) = delete;
	TextRenderClass& operator=(const TextRenderClass& other) = delete;
	TextRenderClass& operator=(TextRenderClass&& other) = delete;
};