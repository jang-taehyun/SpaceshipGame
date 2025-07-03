#include "pch.h"
#include "TextClass.h"
#include "TextManagerClass.h"

bool Text::TextManagerClass::IsInitialize = false;

Text::TextManagerClass::TextManagerClass()
{
	assert(!IsInitialize);
	IsInitialize = true;
}

Text::TextManagerClass::~TextManagerClass()
{
	IsInitialize = false;
}

const Text::ITextClass* Text::TextManagerClass::GetTextObject(TextID ID) const
{
	std::map<TextID, std::unique_ptr<ITextClass>>::const_iterator iter;

	iter = m_UITexts.find(ID);
	assert(m_UITexts.end() != iter);

	return iter->second.get();
}

void Text::TextManagerClass::LoadUIText(const std::wstring& text, TextID id, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT4 color, FontID font)
{
	std::map<TextID, std::unique_ptr<ITextClass>>::iterator iter = m_UITexts.find(id);
	std::unique_ptr<ITextClass> textInst = nullptr;

	if (m_UITexts.end() != iter)
	{
		iter->second->SetText(text);
		iter->second->SetTextPosition(pos);
		iter->second->SetTextColor(color);
		iter->second->SetFontID(font);
		iter->second->SetText(text);

		return;
	}

	textInst = std::make_unique<TextClass>(text, pos, color, font);
	assert(textInst);

	m_UITexts.insert(make_pair(id, std::move(textInst)));
}