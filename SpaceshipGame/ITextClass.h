#pragma once

namespace Text
{
	class ITextClass
	{
	public:
		virtual ~ITextClass() = default;

		virtual inline const std::wstring& GetText() const = 0;
		virtual inline Graphic::Font::ID GetFontID() const = 0;

		virtual inline DirectX::XMFLOAT2 GetPosition() const = 0;
		virtual inline float GetRotation() const = 0;
		virtual inline DirectX::XMFLOAT2 GetOrigin() const = 0;
		virtual inline DirectX::XMFLOAT2 GetScale() const = 0;
		virtual inline DirectX::XMFLOAT4 GetColor() const = 0;

		virtual inline void SetText(const std::wstring& text) = 0;
		virtual inline void SetFontID(Graphic::Font::ID id) = 0;

		virtual inline void SetPosition(DirectX::XMFLOAT2 pos) = 0;
		virtual inline void SetRotation(float rot) = 0;
		virtual inline void SetOrigin(DirectX::XMFLOAT2 origin) = 0;
		virtual inline void SetScale(DirectX::XMFLOAT2 scale) = 0;
		virtual inline void SetColor(DirectX::XMFLOAT4 color) = 0;
	};
}