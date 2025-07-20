#include "pch.h"
#include <DirectXCollision.h>
#include <SimpleMath.h>
#include "InputClass.h"
#include "ButtonClass.h"

UI::ButtonClass::ButtonClass(ID UIID, Graphic::Texture::UITextureID ID) : UIClass(UIID, ID) {}

void UI::ButtonClass::Update(const System::InputClass* input)
{
	int x = 0, y = 0;
	bool IsInCursor = false;
	DirectX::XMFLOAT2 scale = GetScale();

	input->GetMouseLocation(x, y);
	IsInCursor = IsInUI(x, y);

	if (GetUIState() == State::NONE && IsInCursor)
	{
		SetUIState(State::HOVER);
		scale.x *= 1.5f;
		scale.y *= 1.5f;
		SetScale(scale);
	}
	else if (GetUIState() == State::HOVER)
	{
		if(IsInCursor && input->IsMouseLeftBottunPressed())
			SetUIState(State::ONPRESSED);
		else if (!IsInCursor && !input->IsMouseLeftBottunPressed())
		{
			SetUIState(State::NONE);
			scale.x /= 1.5f;
			scale.y /= 1.5f;
			SetScale(scale);
		}
	}
	else if (GetUIState() == State::ONPRESSED && IsInCursor && !input->IsMouseLeftBottunPressed())
	{
		SetUIState(State::ONCLICKED);
	}
	else if(GetUIState() == State::ONCLICKED && !IsInCursor)
	{
		SetUIState(State::NONE);
		scale.x /= 1.5f;
		scale.y /= 1.5f;
		SetScale(scale);
	}
}

bool UI::ButtonClass::IsInUI(int x, int y) const
{
	DirectX::XMFLOAT2 pos = GetPosition();
	DirectX::XMFLOAT2 scale = GetScale();
	DirectX::SimpleMath::Rectangle rect(
		pos.x,
		pos.y,
		scale.x,
		scale.y
	);

	return rect.Contains(x, y);
}
