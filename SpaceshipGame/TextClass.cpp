#include "pch.h"
#include "TextClass.h"

Text::TextClass::TextClass(const std::wstring& text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color, FontID FontID) : m_Text(text), m_Position(pos), m_Color(color), m_FontID(FontID) {}
