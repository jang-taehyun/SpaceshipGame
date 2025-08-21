#pragma once

namespace Text { class ITextClass; }

namespace Text
{
	class TextManagerClass
	{
	public:
		TextManagerClass();
		~TextManagerClass();

		ITextClass* GetTextObject(UINT idx) const;
		UINT GetFontMask() const { return m_FontMask; }
		UINT GetTextCount() const { return static_cast<UINT>(m_UITexts.size()); }

		UINT Load(ID id, const std::wstring& text, Graphic::Font::ID font = Graphic::Font::ID::DEFAULT);
		void Release();

	private:
		static bool IsInitialize;
		
		std::vector<std::unique_ptr<ITextClass>> m_UITexts;
		UINT m_FontMask = 0;

	public:
		TextManagerClass(const TextManagerClass& other) = delete;
		TextManagerClass(TextManagerClass&& other) = delete;
		TextManagerClass& operator=(const TextManagerClass& other) = delete;
		TextManagerClass& operator=(TextManagerClass&& other) = delete;
	};
}