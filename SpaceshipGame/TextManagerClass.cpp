#include "pch.h"
#include "TextClass.h"
#include "TextManagerClass.h"

bool Text::TextManagerClass::IsInitialize = false;

Text::TextManagerClass::TextManagerClass() : m_FontMask(0)
{
	assert(!IsInitialize);
	IsInitialize = true;
}

Text::TextManagerClass::~TextManagerClass()
{
	IsInitialize = false;
}

Text::ITextClass* Text::TextManagerClass::GetTextObject(int idx) const
{
	assert(idx < m_UITexts.size());
	return m_UITexts[idx].get();
}

void Text::TextManagerClass::Load(ID id, const std::wstring& text, Graphic::Font::ID font)
{
	// instance 생성
	std::unique_ptr<ITextClass> textInst = std::make_unique<TextClass>(text, font);
	assert(textInst);

	// 필요한 Font ID 업데이트
	m_FontMask |= (static_cast<UINT>(font));

	// map에 저장
	m_UITexts.push_back(std::move(textInst));
}

void Text::TextManagerClass::Release()
{
	m_UITexts.clear();
	m_FontMask = 0;
}