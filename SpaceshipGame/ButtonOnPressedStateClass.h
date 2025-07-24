#pragma once

#include "IButtonStateClass.h"

namespace UI
{
	class ButtonOnPressedStateClass : public IButtonStateClass
	{
	public:
		ButtonOnPressedStateClass() = default;
		virtual ~ButtonOnPressedStateClass() = default;

		virtual inline ButtonState GetButtonState() const override { return m_ButtonState; }

		virtual std::unique_ptr<IButtonStateClass> Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor) override;

		virtual inline std::unique_ptr<IButtonStateClass> Clone() const override { return std::make_unique<ButtonOnPressedStateClass>(*this); }

	private:
		ButtonState m_ButtonState = ButtonState::ONPRESSED;
	};
}


