#pragma once

#include "IUIClass.h"

namespace UI
{
	class UIClass : public IUIClass
	{
	public:
		UIClass(ID UIID, Graphic::Texture::UITextureID ID);
		virtual ~UIClass() = default;

		virtual ID GetUIID() const { return m_UIID; }
		virtual UIState GetUIState() const override { return m_State; }
		virtual Graphic::Texture::UITextureID GetUITextureID() const override { return m_UITextureID; }

		virtual DirectX::XMFLOAT2 GetPosition() const override { return m_Position; }
		virtual float GetRotation() const override { return m_Rotation; }
		virtual DirectX::XMFLOAT2 GetOrigin() const override { return m_Origin; }
		virtual DirectX::XMFLOAT2 GetScale() const override { return m_Scale; }
		virtual DirectX::XMFLOAT4 GetColor() const override { return m_Color; }

		virtual void SetUIState(UIState state) { m_State = state; }
		virtual void SetUITextureID(Graphic::Texture::UITextureID ID) { m_UITextureID = ID; }

		virtual void SetPosition(DirectX::XMFLOAT2 pos) override { m_Position = pos; }
		virtual void SetRotation(float rot) override { m_Rotation = rot; }
		virtual void SetOrigin(DirectX::XMFLOAT2 origin) override { m_Origin = origin; }
		virtual void SetScale(DirectX::XMFLOAT2 scale) override { m_Scale = scale; }
		virtual void SetColor(DirectX::XMFLOAT4 color) override { m_Color = color; }

		virtual void Update(const System::InputClass* input) override;

	private:
		ID m_UIID = ID::NONE;
		Graphic::Texture::UITextureID m_UITextureID = Graphic::Texture::UITextureID::NONE;
		UIState m_State = UIState::ACTIVE;

		DirectX::XMFLOAT2 m_Position = DirectX::XMFLOAT2(0.f, 0.f);
		float m_Rotation = 0.f;
		DirectX::XMFLOAT2 m_Origin = DirectX::XMFLOAT2(0.f, 0.f);
		DirectX::XMFLOAT2 m_Scale = DirectX::XMFLOAT2(0.f, 0.f);
		DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	};
}
