#include "pch.h"
#include <SimpleMath.h>
#include "D3DClass.h"
#include "ITextClass.h"
#include "UIRenderClass.h"

bool Graphic::UIRenderClass::IsInitialize = false;

Graphic::UIRenderClass::UIRenderClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	assert(!IsInitialize);

	// UI 렌더링을 담당할 sprite batch 생성 //
	m_Renderer = std::make_unique<DirectX::SpriteBatch>(DeviceContext);
	assert(m_Renderer);

	// bitmap 폰트를 메모리에 로드 및 sprite font 생성 //
	m_Font = std::make_unique<DirectX::SpriteFont>(Device, Text::FontFileName.c_str());
	assert(m_Font);

	IsInitialize = true;
}

Graphic::UIRenderClass::~UIRenderClass()
{
	IsInitialize = false;
}

void Graphic::UIRenderClass::BeginRender(const D3DClass* d3d)
{
	// depth buffer 비활성화, alpha blend state 활성화
	d3d->TurnDepthBufferOff();
	d3d->TurnOnAlphaBlending();

	m_Renderer->Begin();
}

void Graphic::UIRenderClass::RenderBackground(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 color)
{
	RECT rect;
	BOOL IsDone = false;

	// 색상 데이터(XMFLOAT4)을 XMVECTOR로 변환 //
	DirectX::XMVECTOR vColor = DirectX::XMLoadFloat4(&color);

	// 현재 윈도우의 크기 가져오기 //
	IsDone = GetWindowRect(GetActiveWindow(), &rect);
	assert(IsDone);

	// 렌더링 //
	m_Renderer->Draw(texture, rect, vColor);
}

void Graphic::UIRenderClass::RenderTexture(ID3D11ShaderResourceView* texture, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color, float rot, DirectX::XMFLOAT2 origin, float scale)
{
	// 색상 데이터(XMFLOAT4)을 XMVECTOR로 변환 //
	DirectX::XMVECTOR vColor = DirectX::XMLoadFloat4(&color);

	// 렌더링 //
	m_Renderer->Draw(texture, pos, nullptr, vColor, rot, origin, scale);
}

void Graphic::UIRenderClass::RenderText(const std::wstring& text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color, float rot, DirectX::XMFLOAT2 origin, float scale)
{
	// 색상 데이터(XMFLOAT4)을 XMVECTOR로 변환 //
	DirectX::XMVECTOR vColor = DirectX::XMLoadFloat4(&color);

	// text의 위치 조정 //
	DirectX::SimpleMath::Vector2 TextPos = pos;
	TextPos.x /= 2.f;
	TextPos.y /= 2.f;

	m_Font->DrawString(m_Renderer.get(), text.c_str(), TextPos, vColor, rot, origin, scale);
}

void Graphic::UIRenderClass::EndRender(const D3DClass* d3d)
{
	m_Renderer->End();

	// alpha blend state 비활성화, depth buffer 활성화
	d3d->TurnOffAlphaBlending();
	d3d->TurnDepthBufferOn();
}