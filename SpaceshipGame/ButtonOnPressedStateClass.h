#pragma once

#include "IButtonStateClass.h"

namespace UI
{
	class ButtonOnPressedStateClass : public IButtonStateClass
	{
	public:
		ButtonOnPressedStateClass(ButtonClass* caller);
		virtual ~ButtonOnPressedStateClass() = default;

		virtual ButtonState GetButtonState() const override { return m_ButtonState; }

		virtual std::unique_ptr<IButtonStateClass> Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor) override;

		virtual std::unique_ptr<IButtonStateClass> Clone() const override { return std::make_unique<ButtonOnPressedStateClass>(*this); }

	private:
		ButtonState m_ButtonState = ButtonState::ONPRESSED;
	};
}


