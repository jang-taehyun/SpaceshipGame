#pragma once

#include <map>

class ITextClass;

class TextManagerClass
{
public:
	TextManagerClass();
	virtual ~TextManagerClass() = default;

	const ITextClass* GetTextObject(int idx) const;
	inline int GetTextObjectCount() const { return m_Texts.size(); }

private:
	static bool IsInitialize;
	std::map<TextFlag, std::unique_ptr<ITextClass>> m_Texts;

public:
	TextManagerClass(const TextManagerClass& other) = delete;
	TextManagerClass(TextManagerClass&& other) = delete;
	TextManagerClass& operator=(const TextManagerClass& other) = delete;
	TextManagerClass& operator=(TextManagerClass&& other) = delete;
};

