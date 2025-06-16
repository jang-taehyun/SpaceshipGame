#include "pch.h"
#include <SimpleMath.h>
#include "D3DClass.h"
#include "TextManagerClass.h"
#include "ITextClass.h"
#include "TextRenderClass.h"

bool TextRenderClass::IsInitialize = false;
static ErrorContent e;

TextRenderClass::TextRenderClass(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext)
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
		throw e;

	IsInitialize = true;
}

TextRenderClass::~TextRenderClass()
{
	IsInitialize = false;
}

HRESULT TextRenderClass::Initialize(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext)
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
	if (!m_SpriteBatch)
	{
		e.contents = _T("sprite batch 객체 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void TextRenderClass::BeginRender(const D3DClass* DirectX3D)
{
	// depth buffer 비활성화, alpha blend state 활성화
	DirectX3D->TurnDepthBufferOff();
	DirectX3D->TurnOnAlphaBlending();

	// Sprite batch 렌더링 시작
	m_SpriteBatch->Begin();
}

void TextRenderClass::Rendering(const ITextClass* Text)
{
	DirectX::SimpleMath::Vector2 FontPos = Text->GetTextPosition();
	DirectX::XMVECTOR FontColor = DirectX::XMLoadFloat4(&(Text->GetTextColor()));

	FontPos.x /= 2.f;
	FontPos.y /= 2.f;

	m_Font->DrawString(m_SpriteBatch.get(), Text->GetText().c_str(), FontPos, FontColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), Text->GetTextSize());
}

void TextRenderClass::EndRender(const D3DClass* DirectX3D)
{
	// Sprite batch 렌더링 종료
	m_SpriteBatch->End();

	// alpha blend state 비활성화, depth buffer 활성화
	DirectX3D->TurnOffAlphaBlending();
	DirectX3D->TurnDepthBufferOn();
}

void TextRenderClass::Render(const D3DClass* DirectX3D, const TextManagerClass* TextManager)
{
	BeginRender(DirectX3D);

	for (int i = 0; i < TextManager->GetTextObjectCount(); ++i)
		if (!(TextManager->GetTextObject(i)))
			Rendering(TextManager->GetTextObject(i));

	EndRender(DirectX3D);
}
