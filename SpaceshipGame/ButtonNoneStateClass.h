#pragma once

#include "IButtonStateClass.h"

namespace UI
{
	class ButtonNoneStateClass : public IButtonStateClass
	{
	public:
		ButtonNoneStateClass() = default;
		explicit ButtonNoneStateClass(ButtonClass* caller);
		virtual ~ButtonNoneStateClass() = default;

		virtual ButtonState GetButtonState() const override { return m_ButtonState; }

		virtual std::unique_ptr<IButtonStateClass> Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor) override;

		virtual std::unique_ptr<IButtonStateClass> Clone() const override { return std::make_unique<ButtonNoneStateClass>(*this); }

	private:
		ButtonState m_ButtonState = ButtonState::NONE;
	};
}


