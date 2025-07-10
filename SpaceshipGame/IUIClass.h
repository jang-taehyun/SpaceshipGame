#pragma once

namespace System { class InputClass; }

namespace UI
{
	class IUIClass
	{
	public:
		virtual ~IUIClass() = default;

		virtual inline ID GetUIID() const = 0;
		virtual inline State GetUIState() const = 0;
		virtual inline Graphic::Texture::UITextureID GetUITextureID() const = 0;

		virtual inline DirectX::XMFLOAT2 GetPosition() const = 0;
		virtual inline float GetRotation() const = 0;
		virtual inline DirectX::XMFLOAT2 GetOrigin() const = 0;
		virtual inline DirectX::XMFLOAT2 GetScale() const = 0;
		virtual inline DirectX::XMFLOAT4 GetColor() const = 0;

		virtual inline void SetUIState(State state) = 0;
		virtual inline void SetUITextureID(Graphic::Texture::UITextureID ID) = 0;

		virtual inline void SetPosition(DirectX::XMFLOAT2 pos) = 0;
		virtual inline void SetRotation(float rot) = 0;
		virtual inline void SetOrigin(DirectX::XMFLOAT2 origin) = 0;
		virtual inline void SetScale(DirectX::XMFLOAT2 scale) = 0;
		virtual inline void SetColor(DirectX::XMFLOAT4 color) = 0;

		virtual void Update(const System::InputClass* input) = 0;
	};
}