#include "pch.h"
#include "TextClass.h"

Text::TextClass::TextClass(const std::wstring& text, Graphic::Font::ID FontID) : m_Text(text), m_FontID(FontID), m_Position(DirectX::XMFLOAT2(0.f, 0.f)), m_Color(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)), m_Size(1.f) {}
