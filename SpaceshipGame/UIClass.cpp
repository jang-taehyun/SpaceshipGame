#include "pch.h"
#include "UIClass.h"

UI::UIClass::UIClass(ID UIID, Graphic::Texture::UITextureID ID) : m_UIID(UIID), m_UITextureID(ID) {}

void UI::UIClass::Update(const System::InputClass* input) {}
