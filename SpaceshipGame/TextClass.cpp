#include "pch.h"
#include "TextClass.h"

TextClass::TextClass(const std::wstring& text, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color) : m_Text(text), m_Position(pos), m_Color(color) {}
