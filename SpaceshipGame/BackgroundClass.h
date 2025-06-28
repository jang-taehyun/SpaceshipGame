#pragma once

#include "IUIClass.h"

namespace UI
{
	class BackgroundClass : public IUIClass
	{
	public:
		BackgroundClass(Graphic::Texture::UITextureID ID, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
		virtual ~BackgroundClass() = default;

		virtual inline DirectX::XMFLOAT2 GetPosition() const override { return m_Position; }
		virtual inline DirectX::XMFLOAT4 GetColor() const override { return m_Color; }
		virtual inline void SetPosition(DirectX::XMFLOAT2 pos) override { m_Position = pos; }
		virtual inline void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }

		virtual void Update(bool IsLeftMouseButtonPressed) override;

	private:
		Graphic::Texture::UITextureID  m_UITextureID = Graphic::Texture::UITextureID::NONE;
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		DirectX::XMFLOAT2 m_Position = DirectX::XMFLOAT2(0.f, 0.f);
	};
}