#include "pch.h"
#include "TextureClass.h"
#include "FontClass.h"

#include <fstream>

FontClass::FontClass() {}
FontClass::FontClass(const FontClass& other) {}
FontClass::~FontClass() {}

bool FontClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FontFileName, const char* TextureFileName)
{
	if (!LoadFontData(FontFileName))
	{
		return false;
	}

	return LoadTexture(Device, DeviceContext, TextureFileName);
}

bool FontClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FontFileName, const TCHAR* tTextureFileName)
{
	if (!LoadFontData(FontFileName))
	{
		return false;
	}

	return LoadTexture(Device, DeviceContext, tTextureFileName);
}

void FontClass::Shutdown()
{
	ReleaseTexture();

	ReleaseFontData();
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* vertices, const char* sentence, float drawX, float drawY)
{
	VertexType* VertexPtr = (VertexType*)vertices;

	int StringLength = (int)strlen(sentence);
	int VertexIndex = 0;
	int CharIndex;

	for (int i = 0; i < StringLength; i++)
	{
		CharIndex = ((int)sentence[i]) - 32;

		// ������ ������ ���ڿ� ó��
		if (CharIndex)
		{
			// First triangle in quad.
			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].left, 0.0f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3((drawX + m_Font[CharIndex].size), (drawY - 16), 0.0f);  // Bottom right.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].right, 1.0f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].left, 1.0f);
			VertexIndex++;

			// Second triangle in quad.
			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].left, 0.0f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(drawX + m_Font[CharIndex].size, drawY, 0.0f);  // Top right.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].right, 0.0f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3((drawX + m_Font[CharIndex].size), (drawY - 16.f), 0.0f);  // Bottom right.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].right, 1.0f);
			VertexIndex++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + m_Font[CharIndex].size + 1.0f;
		}
		// ����(spacebar) ó��
		else
			drawX += 3.f;
	}
}

bool FontClass::LoadFontData(const char* FileName)
{
	std::ifstream FileIn;
	char tmp;

	// file���� ���� font �����͸� ������ �޸� �Ҵ� //
	// file���� font �����ʹ� �� 95�� ����
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// file���� font ������ �б� //
	FileIn.open(FileName);
	if (FileIn.fail())
	{
		return false;
	}

	for (int i = 0; i < 95; i++)
	{
		FileIn.get(tmp);
		while (tmp != ' ')
		{
			FileIn.get(tmp);
		}

		FileIn.get(tmp);
		while (tmp != ' ')
		{
			FileIn.get(tmp);
		}

		FileIn >> m_Font[i].left;
		FileIn >> m_Font[i].right;
		FileIn >> m_Font[i].size;
	}

	// font ���� �ݱ� //
	FileIn.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}
}

bool FontClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	return m_Texture->Initialize(Device, DeviceContext, FileName);
}

bool FontClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tFileName)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	return m_Texture->Initialize(Device, DeviceContext, tFileName);
}

void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
}
