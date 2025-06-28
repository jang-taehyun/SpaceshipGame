#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>

namespace Graphic
{
	class D3DClass;
}

namespace Graphic
{
	class UIRenderClass
	{
	public:
		UIRenderClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
		virtual ~UIRenderClass();

		void BeginRender(const D3DClass* d3d);
		void RenderBackground(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		void RenderTexture(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), float rot = 0.f, DirectX::XMFLOAT2 origin = DirectX::XMFLOAT2(0.f, 0.f), float scale = 1.f);
		void RenderText(const std::wstring& text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), float rot = 0.f, DirectX::XMFLOAT2 origin = DirectX::XMFLOAT2(0.f, 0.f), float scale = 1.f);
		void EndRender(const D3DClass* d3d);

	private:
		static bool IsInitialize;
		std::unique_ptr<DirectX::SpriteBatch> m_Renderer = nullptr;
		std::unique_ptr<DirectX::SpriteFont> m_Font = nullptr;

	public:
		UIRenderClass(const UIRenderClass& other) = delete;
		UIRenderClass(UIRenderClass&& other) = delete;
		UIRenderClass& operator=(const UIRenderClass& other) = delete;
		UIRenderClass& operator=(UIRenderClass&& other) = delete;
	};
}


