#include "pch.h"
#include <SimpleMath.h>
#include "D3DClass.h"
#include "ITextClass.h"
#include "UIRenderClass.h"

bool Graphic::Texture::UIRenderClass::IsInitialize = false;

Graphic::Texture::UIRenderClass::UIRenderClass(ID3D11DeviceContext* DeviceContext) : m_CurrentFontMask(0)
{
	assert(!IsInitialize);

	// UI 렌더링을 담당할 sprite batch 생성 //
	m_Renderer = std::make_unique<DirectX::SpriteBatch>(DeviceContext);
	assert(m_Renderer);

	IsInitialize = true;
}

Graphic::Texture::UIRenderClass::~UIRenderClass()
{
	IsInitialize = false;
}

void Graphic::Texture::UIRenderClass::LoadFont(ID3D11Device* Device, UINT FontMask)
{
	bool IsLoad = false, IsExist = false;
	Font::ID id = Font::ID::NONE;
	UINT flag = 0;
	std::unique_ptr<DirectX::SpriteFont> font = nullptr;
	std::map<Font::ID, const std::wstring>::iterator iter;

	// 필요한 bitmap 폰트를 메모리에 로드 및 sprite font 생성 //
	for (UINT i = 0; i < m_FontIDCount; ++i)
	{
		IsLoad = (FontMask & (1 << i));
		IsExist = (m_CurrentFontMask & (1 << i));
		id = static_cast<Font::ID>(i);

		// 로드를 해야하는데 map에 없는 경우
		if (IsLoad && !IsExist)
		{
			// instance 생성
			iter = m_FontFileList.find(id);
			assert(m_FontFileList.end() != iter);
			font = std::make_unique<DirectX::SpriteFont>(Device, iter->second.c_str());
			assert(font);

			// 현재 로드된 UI texture ID 업데이트
			m_CurrentFontMask |= (1 << i);

			// map에 저장
			m_FontList.insert(std::make_pair(id, std::move(font)));
		}
		// 해제해야 하는데 map에 있는 경우
		else if (!IsLoad && IsExist)
		{
			// instance가 실제로 존재하는지 확인하고 해제
			if (m_FontList.end() != m_FontList.find(id))
				m_FontList.erase(id);

			// 현재 로드된 UI texture ID 제거
			flag = ~(1 << i);
			m_CurrentFontMask &= flag;
		}
	}
}

void Graphic::Texture::UIRenderClass::BeginRender(const D3DClass* d3d)
{
	// depth buffer 비활성화, alpha blend state 활성화
	d3d->TurnDepthBufferOff();
	d3d->TurnOnAlphaBlending();

	m_Renderer->Begin(DirectX::DX11::SpriteSortMode_FrontToBack);
}

void Graphic::Texture::UIRenderClass::RenderBackground(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 color)
{
	// 색상 데이터(XMFLOAT4)을 XMVECTOR로 변환 //
	DirectX::XMVECTOR vColor = DirectX::XMLoadFloat4(&color);

	// texture와 UI의 위치 조정 //
	RECT rect;
	GetClientRect(System::hwnd, &rect);

	// 렌더링 //
	m_Renderer->Draw(
		texture,
		rect,
		nullptr,
		vColor
	);
}

void Graphic::Texture::UIRenderClass::RenderTexture(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color, float rot, DirectX::XMFLOAT2 origin, DirectX::XMFLOAT2 scale, float depth)
{
	// 색상 데이터(XMFLOAT4)을 XMVECTOR로 변환 //
	DirectX::XMVECTOR vColor(DirectX::XMLoadFloat4(&color));
	
	// texture와 UI의 위치 조정 //
	RECT rect = {
		static_cast<LONG>(pos.x - scale.x * 0.5f),
		static_cast<LONG>(pos.y - scale.y * 0.5f),
		static_cast<LONG>(pos.x + scale.x * 0.5f),
		static_cast<LONG>(pos.y + scale.y * 0.5f)
	};

	// 렌더링 //
	m_Renderer->Draw(
		texture,
		rect,
		nullptr,
		vColor,
		rot,
		origin,
		DirectX::DX11::SpriteEffects_None,
		depth
	);
}

void Graphic::Texture::UIRenderClass::RenderText(const std::wstring& text, Font::ID fontID, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color, float rot, DirectX::XMFLOAT2 origin, DirectX::XMFLOAT2 scale, float depth)
{
	std::map<Font::ID, std::unique_ptr<DirectX::SpriteFont>>::iterator iter;
	DirectX::XMVECTOR size;
	DirectX::XMFLOAT2 TextOrigin;

	// 색상 데이터(XMFLOAT4)을 XMVECTOR로 변환 //
	DirectX::XMVECTOR vColor(DirectX::XMLoadFloat4(&color));

	// font list에서 font를 찾기
	iter = m_FontList.find(fontID);
	assert(m_FontList.end() != iter);

	// text의 위치 조정 //
	size = iter->second->MeasureString(text.c_str());
	DirectX::XMStoreFloat2(&TextOrigin, size);
	TextOrigin.x /= 2;
	TextOrigin.y /= 2;

	// 렌더링
	iter->second->DrawString(
		m_Renderer.get(),
		text.c_str(),
		pos,
		vColor,
		rot,
		TextOrigin,
		scale,
		DirectX::DX11::SpriteEffects_None,
		depth
	);
}

void Graphic::Texture::UIRenderClass::EndRender(const D3DClass* d3d)
{
	m_Renderer->End();

	// alpha blend state 비활성화, depth buffer 활성화
	d3d->TurnOffAlphaBlending();
	d3d->TurnDepthBufferOn();
}