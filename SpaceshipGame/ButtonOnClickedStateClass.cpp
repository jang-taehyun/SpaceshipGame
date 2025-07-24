#include "pch.h"
#include "ButtonClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonOnClickedStateClass.h"

UI::ButtonOnClickedStateClass::ButtonOnClickedStateClass(ButtonClass* caller)
{
	DirectX::XMFLOAT2 scale = caller->GetScale();

	scale.x /= 1.5f;
	scale.y /= 1.5f;
	caller->SetScale(scale);
}

std::unique_ptr<UI::IButtonStateClass> UI::ButtonOnClickedStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
    return nullptr;
}
