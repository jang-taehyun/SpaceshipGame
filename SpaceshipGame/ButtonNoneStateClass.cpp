#include "pch.h"
#include "InputClass.h"
#include "ButtonClass.h"
#include "ButtonHoverStateClass.h"
#include "ButtonNoneStateClass.h"

UI::ButtonNoneStateClass::ButtonNoneStateClass(ButtonClass* caller)
{
	DirectX::XMFLOAT2 scale = caller->GetScale();
	
	scale.x /= ExtendSize;
	scale.y /= ExtendSize;
	caller->SetScale(scale);

#ifdef _DEBUG
	OutputDebugStringA("NONE\n");
#endif // _DEBUG
}

std::unique_ptr<UI::IButtonStateClass> UI::ButtonNoneStateClass::Update(ButtonClass* caller, const System::InputClass* input, bool IsInCursor)
{
	std::unique_ptr<IButtonStateClass> ret = nullptr;
	
	if (IsInCursor)
		ret = std::make_unique<ButtonHoverStateClass>(caller);

	return ret;
}
