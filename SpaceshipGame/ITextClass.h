#pragma once

namespace Text
{
	class ITextClass
	{
	public:
		virtual ~ITextClass() = default;

		virtual const std::wstring& GetText() const = 0;
		virtual Graphic::Font::ID GetFontID() const = 0;
		virtual UINT GetTextState() const = 0;

		virtual DirectX::XMFLOAT2 GetPosition() const = 0;
		virtual float GetRotation() const = 0;
		virtual DirectX::XMFLOAT2 GetOrigin() const = 0;
		virtual DirectX::XMFLOAT2 GetScale() const = 0;
		virtual DirectX::XMFLOAT4 GetColor() const = 0;
		virtual float GetDepth() const = 0;

		virtual void SetText(const std::wstring& text) = 0;
		virtual void SetFontID(Graphic::Font::ID id) = 0;
		virtual void SetTextState(UI::UIState state, bool IsActive) = 0;

		virtual void SetPosition(DirectX::XMFLOAT2 pos) = 0;
		virtual void SetRotation(float rot) = 0;
		virtual void SetOrigin(DirectX::XMFLOAT2 origin) = 0;
		virtual void SetScale(DirectX::XMFLOAT2 scale) = 0;
		virtual void SetColor(DirectX::XMFLOAT4 color) = 0;
		virtual void SetDepth(float depth) = 0;
	};
}