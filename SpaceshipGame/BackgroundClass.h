#pragma once

#include "UIClass.h"

namespace UI
{
	class BackgroundClass : public UIClass
	{
	public:
		BackgroundClass(Graphic::Texture::UITextureID ID, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		virtual ~BackgroundClass() = default;

		virtual void Update(const System::InputClass* input) override;
	};
}