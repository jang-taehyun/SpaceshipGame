#include "pch.h"
#include "UIClass.h"

UI::UIClass::UIClass(Graphic::Texture::UITextureID ID) : m_Position(DirectX::XMFLOAT2(0.f, 0.f)), m_Color(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f)), m_UITextureID(ID), m_State(State::NONE) {}

void UI::UIClass::Update(const System::InputClass* input) {}
