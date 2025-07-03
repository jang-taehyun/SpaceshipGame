#pragma once

namespace Text
{
	class ITextClass
	{
	public:
		virtual ~ITextClass() = default;

		virtual inline const std::wstring& GetText() const = 0;
		virtual inline FontID GetFontID() const = 0;
		virtual inline DirectX::XMFLOAT2 GetTextPosition() const = 0;
		virtual inline DirectX::XMFLOAT4 GetTextColor() const = 0;
		virtual inline float GetTextSize() const = 0;

		virtual inline void SetText(const std::wstring& text) = 0;
		virtual inline void SetFontID(FontID id) = 0;
		virtual inline void SetTextPosition(DirectX::XMFLOAT2 pos) = 0;
		virtual inline void SetTextColor(DirectX::XMFLOAT4 color) = 0;
		virtual inline void SetTextSize(float size) = 0;

		virtual inline std::unique_ptr<ITextClass> Clone() const = 0;
	};
}