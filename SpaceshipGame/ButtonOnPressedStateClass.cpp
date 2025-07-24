#include "pch.h"
#include "InputClass.h"
#include "ButtonClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonOnClickedStateClass.h"
#include "ButtonOnPressedStateClass.h"

std::unique_ptr<UI::IButtonStateClass> UI::ButtonOnPressedStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
	std::unique_ptr<IButtonStateClass> ret = nullptr;
	DirectX::XMFLOAT2 scale;

	if (IsInCursor && !input->IsMouseLeftBottunPressed())
	{
		ret = std::make_unique<ButtonOnClickedStateClass>(caller);

		scale = caller->GetScale();
		scale.x /= 1.5f;
		scale.y /= 1.5f;
		caller->SetScale(scale);
	}

	return ret;
}
