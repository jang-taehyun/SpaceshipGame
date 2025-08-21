#include "pch.h"
#include "ButtonClass.h"
#include "ButtonNoneStateClass.h"
#include "ButtonOnClickedStateClass.h"

UI::ButtonOnClickedStateClass::ButtonOnClickedStateClass(ButtonClass* caller)
{
	DirectX::XMFLOAT2 scale = caller->GetScale();

	scale.x /= ExtendSize;
	scale.y /= ExtendSize;
	caller->SetScale(scale);

#ifdef _DEBUG
	OutputDebugStringA("ON CLICKED\n");
#endif // _DEBUG
}

std::unique_ptr<UI::IButtonStateClass> UI::ButtonOnClickedStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
    return std::make_unique<ButtonNoneStateClass>(caller);
}
