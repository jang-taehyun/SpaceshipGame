#include "pch.h"
#include "ButtonClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonOnClickedStateClass.h"

UI::ButtonOnClickedStateClass::ButtonOnClickedStateClass(ButtonClass* caller)
{
#ifdef _DEBUG
	OutputDebugStringA("ON CLICKED\n");
#endif // _DEBUG
}

std::unique_ptr<UI::IButtonStateClass> UI::ButtonOnClickedStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
    return std::make_unique<ButtonNoneStateClass>();
}
