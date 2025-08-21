#pragma once

namespace System { class InputClass; }

namespace UI
{
	class IUIClass
	{
	public:
		virtual ~IUIClass() = default;

		virtual ID GetUIID() const = 0;
		virtual UIState GetUIState() const = 0;
		virtual Graphic::Texture::UITextureID GetUITextureID() const = 0;

		virtual DirectX::XMFLOAT2 GetPosition() const = 0;
		virtual float GetRotation() const = 0;
		virtual DirectX::XMFLOAT2 GetOrigin() const = 0;
		virtual DirectX::XMFLOAT2 GetScale() const = 0;
		virtual DirectX::XMFLOAT4 GetColor() const = 0;

		virtual void SetUIState(UIState state) = 0;
		virtual void SetUITextureID(Graphic::Texture::UITextureID ID) = 0;

		virtual void SetPosition(DirectX::XMFLOAT2 pos) = 0;
		virtual void SetRotation(float rot) = 0;
		virtual void SetOrigin(DirectX::XMFLOAT2 origin) = 0;
		virtual void SetScale(DirectX::XMFLOAT2 scale) = 0;
		virtual void SetColor(DirectX::XMFLOAT4 color) = 0;

		virtual void Update(const System::InputClass* input) = 0;
	};
}