#include "pch.h"
#include "FontClass.h"

#include <fstream>

FontClass::FontClass() {}
FontClass::FontClass(const FontClass& other) {}
FontClass::~FontClass() {}

HRESULT FontClass::Initialize(const ID3D11Device* const& Device, const ID3D11DeviceContext* const& DeviceContext, const tstring& FontFileName, const tstring& TextureFileName)
{
	if (FAILED(LoadFontData(FontFileName)))
	{
		return E_FAIL;
	}

	return LoadTexture(Device, DeviceContext, TextureFileName);
}

void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

void FontClass::BuildVertexArray(const void* const& vertices, const tstring& sentence, const float& drawX, const float& drawY)
{
	VertexType* VertexPtr = (VertexType*)vertices;

	int StringLength = (int)sentence.length();
	int VertexIndex = 0;
	int CharIndex;

	float x = drawX, y = drawY;

	for (int i = 0; i < StringLength; ++i)
	{
		CharIndex = ((int)sentence[i]) - 32;

		// ������ ������ ���ڿ� ó��
		if (CharIndex)
		{
			// First triangle in quad.
			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(x, y, 0.f);  // Top left.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].left, 0.f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3((x + m_Font[CharIndex].size), (y - 16), 0.f);  // Bottom right.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].right, 1.f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(x, (y - 16), 0.f);  // Bottom left.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].left, 1.f);
			VertexIndex++;

			// Second triangle in quad.
			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(x, y, 0.f);  // Top left.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].left, 0.f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3(x + m_Font[CharIndex].size, y, 0.f);  // Top right.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].right, 0.f);
			VertexIndex++;

			VertexPtr[VertexIndex].position = DirectX::XMFLOAT3((drawX + m_Font[CharIndex].size), (y - 16.f), 0.f);  // Bottom right.
			VertexPtr[VertexIndex].texture = DirectX::XMFLOAT2(m_Font[CharIndex].right, 1.f);
			VertexIndex++;

			// Update the x location for drawing by the size of the letter and one pixel.
			x = x + m_Font[CharIndex].size + 1.0f;
		}
		// ����(spacebar) ó��
		else
			x += 3.f;
	}
}

HRESULT FontClass::LoadFontData(const tstring& FileName)
{
	std::ifstream FileIn;
	char tmp;

	// file���� ���� font �����͸� ������ �޸� �Ҵ� //
	// file���� font �����ʹ� �� 95�� ����
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return E_FAIL;
	}

	// file���� font ������ �б� //
	FileIn.open(FileName);
	if (FileIn.fail())
	{
		return E_FAIL;
	}

	for (int i = 0; i < 95; ++i)
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

	return S_OK;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = nullptr;
	}
}

HRESULT FontClass::LoadTexture(const ID3D11Device* const& Device, const ID3D11DeviceContext* const& DeviceContext, const tstring& FileName)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return E_FAIL;
	}

	return m_Texture->Initialize(Device, DeviceContext, FileName);
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
