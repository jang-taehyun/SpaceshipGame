#include "pch.h"
#include "InputClass.h"
#include "ButtonClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonOnClickedStateClass.h"
#include "ButtonOnPressedStateClass.h"

UI::ButtonOnPressedStateClass::ButtonOnPressedStateClass()
{
#ifdef _DEBUG
	OutputDebugStringA("ON PRESSED\n");
#endif // _DEBUG
}

std::unique_ptr<UI::IButtonStateClass> UI::ButtonOnPressedStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
	std::unique_ptr<IButtonStateClass> ret = nullptr;

	if (IsInCursor && !input->IsMouseLeftBottunPressed())
		ret = std::make_unique<ButtonOnClickedStateClass>(caller);
	else if(!IsInCursor)
		ret = std::make_unique<ButtonNoneStateClass>(caller);

	return ret;
}
