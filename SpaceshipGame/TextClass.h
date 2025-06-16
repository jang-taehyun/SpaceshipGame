#pragma once

#include "ITextClass.h"

class TextClass : public ITextClass
{
public:
	TextClass(const std::wstring& text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color);
	virtual ~TextClass() = default;

	virtual inline const std::wstring& GetText() const override { return m_Text; }
	virtual inline DirectX::XMFLOAT2 GetTextPosition() const override { return m_Position; }
	virtual inline DirectX::XMFLOAT4 GetTextColor() const override { return m_Color; }
	virtual inline float GetTextSize() const override { return m_Size; }

	virtual inline void SetText(const std::wstring& text) override { m_Text = text; }
	virtual inline void SetTextPosition(DirectX::XMFLOAT2 pos) override { m_Position = pos; }
	virtual inline void SetTextColor(DirectX::XMFLOAT4 color) override { m_Color = color; }
	virtual inline float SetTextSize(float size) override { m_Size = size; }

	virtual inline std::unique_ptr<ITextClass> Clone() const override { return std::make_unique<TextClass>(*this); }

private:
	std::wstring m_Text = _T("");
	DirectX::XMFLOAT2 m_Position = DirectX::XMFLOAT2(0.f, 0.f);
	DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	float m_Size = 1.f;
};

