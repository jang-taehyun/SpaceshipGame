#include "pch.h"
#include <SimpleMath.h>
#include "D3DClass.h"
#include "TextRenderClass.h"

bool TextRenderClass::IsInitialize = false;
static ErrorContent e;

TextRenderClass::TextRenderClass(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextRenderClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 TextRenderClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize(Device, DeviceContext);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}

	IsInitialize = true;
}

TextRenderClass::~TextRenderClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT TextRenderClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextRenderClass Initialize()");

	// bitmap 폰트를 로드 //
	m_Font = std::make_unique<DirectX::SpriteFont>(Device, FontFileName.c_str());
	if (!m_Font.get())
	{
		e.contents = _T("sprite font 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// bitmap 폰트를 draw할 SpriteBatch 객체 생성 //
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(DeviceContext);
	if (!m_SpriteBatch.get())
	{
		e.contents = _T("sprite batch 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void TextRenderClass::Shutdown()
{
	if (m_SpriteBatch)
	{
		m_SpriteBatch.reset();
	}
	
	if (m_Font)
	{
		m_Font.reset();
	}
}

void TextRenderClass::BeginRender(D3DClass* const& DirectX3D)
{
	// depth buffer 비활성화, alpha blend state 활성화
	DirectX3D->TurnDepthBufferOff();
	DirectX3D->TurnOnAlphaBlending();

	// Sprite batch 렌더링 시작
	m_SpriteBatch->Begin();
}

void TextRenderClass::Rendering(const std::wstring& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color)
{
	DirectX::SimpleMath::Vector2 FontPos = pos;
	DirectX::XMVECTOR FontColor = DirectX::XMLoadFloat4(&color);
	FontPos.x /= 2.f;
	FontPos.y /= 2.f;

	m_Font->DrawString(m_SpriteBatch.get(), text.c_str(), FontPos, FontColor);
}

void TextRenderClass::EndRender(D3DClass* const& DirectX3D)
{
	// Sprite batch 렌더링 종료
	m_SpriteBatch->End();

	// alpha blend state 비활성화, depth buffer 활성화
	DirectX3D->TurnOffAlphaBlending();
	DirectX3D->TurnDepthBufferOn();
}

void TextRenderClass::Render(D3DClass* const& DirectX3D, const std::wstring& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT4& color)
{
	BeginRender(DirectX3D);
	Rendering(text, pos, color);
	EndRender(DirectX3D);
}
