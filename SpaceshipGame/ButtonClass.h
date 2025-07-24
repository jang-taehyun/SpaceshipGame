#pragma once

#include "UIClass.h"

namespace UI { class IButtonStateClass; }

namespace UI
{
	class ButtonClass : public UIClass
	{
	public:
		ButtonClass(ID UIID, Graphic::Texture::UITextureID ID);
		virtual ~ButtonClass() = default;
		ButtonClass(const ButtonClass& other);
		ButtonClass(ButtonClass&& other) noexcept;

		ButtonClass& operator=(const ButtonClass&& other);
		ButtonClass& operator=(ButtonClass&& other) noexcept;

		virtual void Update(const System::InputClass* input) override;

		ButtonState GetButtonState() const;

	private:
		bool IsInUI(int x, int y) const;

	private:
		std::unique_ptr<IButtonStateClass> m_ButtonState = nullptr;
	};
}