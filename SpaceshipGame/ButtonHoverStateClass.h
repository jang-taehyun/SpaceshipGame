#pragma once

#include "IButtonStateClass.h"

namespace UI
{
	class ButtonHoverStateClass : public IButtonStateClass
	{
	public:
		explicit ButtonHoverStateClass(ButtonClass* caller);
		virtual ~ButtonHoverStateClass() = default;

		virtual inline ButtonState GetButtonState() const override { return m_ButtonState; }

		virtual std::unique_ptr<IButtonStateClass> Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor) override;

		virtual inline std::unique_ptr<IButtonStateClass> Clone() const override { return std::make_unique<ButtonHoverStateClass>(*this); }

	private:
		ButtonState m_ButtonState = ButtonState::HOVER;
	};
}


