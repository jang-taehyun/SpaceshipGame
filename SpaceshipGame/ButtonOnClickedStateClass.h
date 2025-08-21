#pragma once

#include "IButtonStateClass.h"

namespace UI
{
	class ButtonOnClickedStateClass : public IButtonStateClass
	{
	public:
		explicit ButtonOnClickedStateClass(ButtonClass* caller);
		virtual ~ButtonOnClickedStateClass() = default;

		virtual ButtonState GetButtonState() const override { return m_ButtonState; }

		virtual std::unique_ptr<IButtonStateClass> Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor) override;

		virtual std::unique_ptr<IButtonStateClass> Clone() const override { return std::make_unique<ButtonOnClickedStateClass>(*this); }

	private:
		ButtonState m_ButtonState = ButtonState::ONCLICKED;
	};
}