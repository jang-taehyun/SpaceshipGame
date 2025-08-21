#pragma once

#include "ITextClass.h"

namespace Text
{
	class TextClass : public ITextClass
	{
	public:
		TextClass(const std::wstring& text, Graphic::Font::ID FontID = Graphic::Font::ID::DEFAULT);
		virtual ~TextClass() = default;

		virtual const std::wstring& GetText() const override { return m_Text; }
		virtual Graphic::Font::ID GetFontID() const override { return m_FontID; }

		virtual DirectX::XMFLOAT2 GetPosition() const override { return m_Position; }
		virtual float GetRotation() const override { return m_Rotation; }
		virtual DirectX::XMFLOAT2 GetOrigin() const override { return m_Origin; }
		virtual DirectX::XMFLOAT2 GetScale() const override { return m_Scale; }
		virtual DirectX::XMFLOAT4 GetColor() const override { return m_Color; }

		virtual void SetText(const std::wstring& text) override { m_Text = text; }
		virtual void SetFontID(Graphic::Font::ID id) override { m_FontID = id; }
		
		virtual void SetPosition(DirectX::XMFLOAT2 pos) override { m_Position = pos; }
		virtual void SetRotation(float rot) override { m_Rotation = rot; }
		virtual void SetOrigin(DirectX::XMFLOAT2 origin) override { m_Origin = origin; }
		virtual void SetScale(DirectX::XMFLOAT2 scale) override { m_Scale = scale; }
		virtual void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }

	private:
		std::wstring m_Text = _T("");
		Graphic::Font::ID m_FontID = Graphic::Font::ID::NONE;

		DirectX::XMFLOAT2 m_Position = DirectX::XMFLOAT2(0.f, 0.f);
		float m_Rotation = 0.f;
		DirectX::XMFLOAT2 m_Origin = DirectX::XMFLOAT2(0.f, 0.f);
		DirectX::XMFLOAT2 m_Scale = DirectX::XMFLOAT2(0.f, 0.f);
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	};
}