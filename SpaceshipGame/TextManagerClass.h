#pragma once

namespace Text { class ITextClass; }

namespace Text
{
	class TextManagerClass
	{
	public:
		TextManagerClass();
		virtual ~TextManagerClass();

		const ITextClass* GetTextObject(TextID ID) const;
		inline UINT GetChatTextsCount() const { return static_cast<UINT>(m_ChatTexts.size()); }

		void LoadUIText(const std::wstring& text, TextID id, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f), FontID font = FontID::Default);

	private:
		static bool IsInitialize;
		
		std::map<TextID, std::unique_ptr<ITextClass>> m_UITexts;
		std::vector<std::unique_ptr<ITextClass>> m_ChatTexts;
		UINT m_Loaded = 0;

	public:
		TextManagerClass(const TextManagerClass& other) = delete;
		TextManagerClass(TextManagerClass&& other) = delete;
		TextManagerClass& operator=(const TextManagerClass& other) = delete;
		TextManagerClass& operator=(TextManagerClass&& other) = delete;
	};

}