#pragma once

namespace Text { class ITextClass; }

namespace Text
{
	class TextManagerClass
	{
	public:
		TextManagerClass();
		virtual ~TextManagerClass();

		const ITextClass* GetTextObject(int idx) const;
		inline UINT GetTextObjectCount() const { return static_cast<UINT>(m_Texts.size()); }

	private:
		static bool IsInitialize;
		std::vector<std::unique_ptr<ITextClass>> m_Texts;

	public:
		TextManagerClass(const TextManagerClass& other) = delete;
		TextManagerClass(TextManagerClass&& other) = delete;
		TextManagerClass& operator=(const TextManagerClass& other) = delete;
		TextManagerClass& operator=(TextManagerClass&& other) = delete;
	};

}