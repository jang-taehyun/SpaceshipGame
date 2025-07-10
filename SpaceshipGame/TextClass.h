#pragma once

#include "ITextClass.h"

namespace Text
{
	class TextClass : public ITextClass
	{
	public:
		TextClass(const std::wstring& text, Graphic::Font::ID FontID = Graphic::Font::ID::DEFAULT);
		virtual ~TextClass() = default;

		virtual inline const std::wstring& GetText() const override { return m_Text; }
		virtual inline Graphic::Font::ID GetFontID() const override { return m_FontID; }

		virtual inline DirectX::XMFLOAT2 GetPosition() const override { return m_Position; }
		virtual inline float GetRotation() const override { return m_Rotation; }
		virtual inline DirectX::XMFLOAT2 GetOrigin() const override { return m_Origin; }
		virtual inline DirectX::XMFLOAT2 GetScale() const override { return m_Scale; }
		virtual inline DirectX::XMFLOAT4 GetColor() const override { return m_Color; }

		virtual inline void SetText(const std::wstring& text) override { m_Text = text; }
		virtual inline void SetFontID(Graphic::Font::ID id) override { m_FontID = id; }
		
		virtual inline void SetPosition(DirectX::XMFLOAT2 pos) override { m_Position = pos; }
		virtual inline void SetRotation(float rot) override { m_Rotation = rot; }
		virtual inline void SetOrigin(DirectX::XMFLOAT2 origin) override { m_Origin = origin; }
		virtual inline void SetScale(DirectX::XMFLOAT2 scale) override { m_Scale = scale; }
		virtual inline void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }

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