#include "pch.h"
#include "ITextClass.h"
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

const Text::ITextClass* Text::TextManagerClass::GetTextObject(int idx) const
{
	assert(idx < m_Texts.size());
	return m_Texts[idx].get();
}