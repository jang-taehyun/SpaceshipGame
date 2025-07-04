#include "pch.h"
#include "BackgroundClass.h"

UI::BackgroundClass::BackgroundClass(Graphic::Texture::UITextureID ID) : UIClass(ID)
{
	RECT rect = {};
	int width = 0, height = 0, PosX = 0, PosY = 0;

	if (System::FULL_SCREEN)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		width = System::WIDTH;
		height = System::HEIGHT;
	}
	
	GetWindowRect(GetActiveWindow(), &rect);

	PosX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	PosY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	SetPosition(DirectX::XMFLOAT2(static_cast<float>(PosX), static_cast<float>(PosY)));
}

void UI::BackgroundClass::Update(const System::InputClass* input) {}
