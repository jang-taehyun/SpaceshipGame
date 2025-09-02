#include "pch.h"
#include <DirectXCollision.h>
#include <SimpleMath.h>
#include "InputClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonClass.h"

UI::ButtonClass::ButtonClass(ID UIID, Graphic::Texture::UITextureID ID)
	: UIClass(UIID, ID)
{
	m_ButtonState = std::make_unique<ButtonNoneStateClass>();
	assert(m_ButtonState);
}

UI::ButtonClass::ButtonClass(const ButtonClass& other)
	: UIClass(other)
{
	m_ButtonState = std::move(other.m_ButtonState->Clone());
}

UI::ButtonClass::ButtonClass(ButtonClass&& other) noexcept
	: UIClass(other)
{
	m_ButtonState = std::move(other.m_ButtonState);
}

UI::ButtonClass& UI::ButtonClass::operator=(const ButtonClass& other)
{
	if (this == &other)
		return *this;

	if (m_ButtonState)
		m_ButtonState.reset();
	m_ButtonState = std::move(other.m_ButtonState->Clone());

	UIClass::operator=(other);

	return *this;
}

UI::ButtonClass& UI::ButtonClass::operator=(ButtonClass&& other) noexcept
{
	if (this == &other)
		return *this;

	if (m_ButtonState)
		m_ButtonState.reset();
	m_ButtonState = std::move(other.m_ButtonState);

	UIClass::operator=(other);

	return *this;
}

void UI::ButtonClass::Update(const System::InputClass* input)
{
	int x = 0, y = 0;
	bool IsInCursor = false;
	std::unique_ptr<IButtonStateClass> ret = nullptr;
	
	if (GetUIState() & (1 << static_cast<UINT>(UIState::ACTIVE)))
	{
		input->GetMouseLocation(x, y);
		IsInCursor = IsInUI(x, y);

		ret = std::move(m_ButtonState->Update(this, input, IsInCursor));
		if (ret)
		{
			m_ButtonState.reset();
			m_ButtonState = std::move(ret);
		}
	}
}

UI::ButtonState UI::ButtonClass::GetButtonState() const
{
	return m_ButtonState->GetButtonState();
}

bool UI::ButtonClass::IsInUI(int x, int y) const
{
	DirectX::XMFLOAT2 pos = GetPosition();
	DirectX::XMFLOAT2 scale = GetScale();
	RECT rect =
	{
		static_cast<long>(pos.x - scale.x / 2),
		static_cast<long>(pos.y - scale.y / 2),
		static_cast<long>(pos.x + scale.x / 2),
		static_cast<long>(pos.y + scale.y / 2)
	};

	return DirectX::SimpleMath::Rectangle(rect).Contains(x, y);
}
