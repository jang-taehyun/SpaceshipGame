#include "pch.h"
#include "InputClass.h"
#include "ButtonClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonOnPressedStateClass.h"
#include "ButtonHoverStateClass.h"

UI::ButtonHoverStateClass::ButtonHoverStateClass(ButtonClass* caller)
{
	DirectX::XMFLOAT2 scale;

	scale = caller->GetScale();
	scale.x *= ExtendSize;
	scale.y *= ExtendSize;
	caller->SetScale(scale);

#ifdef _DEBUG
	OutputDebugStringA("HOVER\n");
#endif // _DEBUG
}

std::unique_ptr<UI::IButtonStateClass> UI::ButtonHoverStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
	std::unique_ptr<IButtonStateClass> ret = nullptr;
	
	if (IsInCursor && input->IsMouseLeftBottunPressed())
		ret = std::make_unique<ButtonOnPressedStateClass>();
	else if (!IsInCursor)
		ret = std::make_unique<ButtonNoneStateClass>(caller);

	return ret;
}

