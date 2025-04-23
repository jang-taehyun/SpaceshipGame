#include "pch.h"
#include <SimpleMath.h>
#include "TextClass.h"

bool TextClass::IsInitialize = false;
static ErrorContent e;

TextClass::TextClass(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 TextClass 인스턴스가 존재합니다.");
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

TextClass::~TextClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT TextClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextClass Initialize()");

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

void TextClass::Shutdown()
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

void TextClass::Render(ID3D11DeviceContext* const& DeviceContext, const std::wstring& text, const DirectX::XMFLOAT2& pos, const DirectX::XMVECTOR& color)
{
	DirectX::SimpleMath::Vector2 FontPos = pos;
	FontPos.x /= 2.f;
	FontPos.y /= 2.f;

	m_SpriteBatch->Begin();

	m_Font->DrawString(m_SpriteBatch.get(), text.c_str(), FontPos, color);

	m_SpriteBatch->End();
}