#pragma once

#include "UIClass.h"

namespace UI
{
	class ButtonClass : public UIClass
	{
	public:
		ButtonClass(ID UIID, Graphic::Texture::UITextureID ID);
		virtual ~ButtonClass() = default;

		virtual void Update(const System::InputClass* input) override;

	private:
		bool IsInUI(int x, int y) const;
	};
}