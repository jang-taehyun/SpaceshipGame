#pragma once

#include "UIClass.h"

namespace UI
{
	class BackgroundClass : public UIClass
	{
	public:
		BackgroundClass(Graphic::Texture::UITextureID ID);
		virtual ~BackgroundClass() = default;

		virtual void Update(const System::InputClass* input) override;
	};
}