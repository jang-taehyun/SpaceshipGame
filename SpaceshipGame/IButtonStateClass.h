#pragma once

namespace System { class InputClass; }
namespace UI { class ButtonClass; }

namespace UI
{
	class IButtonStateClass
	{
	public:
		virtual ~IButtonStateClass() = default;

		virtual inline ButtonState GetButtonState() const = 0;

		virtual std::unique_ptr<IButtonStateClass> Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor) = 0;

		virtual inline std::unique_ptr<IButtonStateClass> Clone() const = 0;
	};
}
