#include "pch.h"
#include "TextClass.h"

Text::TextClass::TextClass(const std::wstring& text, Graphic::Font::ID FontID) : m_Text(text), m_FontID(FontID) {}

void Text::TextClass::SetTextState(UI::UIState state, bool IsActive)
{
	if (IsActive)
		m_State |= (1 << static_cast<UINT>(state));
	else
		m_State &= ~(1 << static_cast<UINT>(state));
}
