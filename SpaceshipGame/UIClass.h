#pragma once

#include "IUIClass.h"

namespace UI
{
	class UIClass : public IUIClass
	{
	public:
		UIClass(Graphic::Texture::UITextureID ID, DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		virtual ~UIClass() = default;

		virtual inline DirectX::XMFLOAT2 GetPosition() const override { return m_Position; }
		virtual inline DirectX::XMFLOAT4 GetColor() const override { return m_Color; }

		virtual inline void SetPosition(DirectX::XMFLOAT2 pos) override { m_Position = pos; }
		virtual inline void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }

		virtual inline State GetUIState() const override { return m_State; }
		virtual inline Graphic::Texture::UITextureID GetUIID() const override { return m_UITextureID; }

		virtual void Update(const System::InputClass* input) override = 0;

	private:
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		DirectX::XMFLOAT2 m_Position = DirectX::XMFLOAT2(0.f, 0.f);

		Graphic::Texture::UITextureID m_UITextureID = Graphic::Texture::UITextureID::NONE;
		State m_State = State::NONE;
	};
}
