#include "pch.h"
#include "UIClass.h"

UI::UIClass::UIClass(ID UIID, Graphic::Texture::UITextureID ID) : m_UIID(UIID), m_UITextureID(ID) {}

void UI::UIClass::SetUIState(UIState state, bool IsActive)
{
	if (IsActive)
		m_State |= (1 << static_cast<UINT>(state));
	else
		m_State &= ~(1 << static_cast<UINT>(state));
}

void UI::UIClass::Update(const System::InputClass* input) {}
