#include "pch.h"
#include "UIClass.h"

UI::UIClass::UIClass(Graphic::Texture::UITextureID ID, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color) : m_UITextureID(ID), m_Position(position), m_Color(color) {}

void UI::UIClass::Update(bool IsLeftMouseButtonPressed) {}
