#pragma once

#include "TextureClass.h"

class FontClass
{
private:
	struct FontType
	{
		float left;
		float right;
		int size;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass& other);
	~FontClass();

	inline const ID3D11ShaderResourceView* const& GetTexture(const int idx) const { return (m_Texture->GetTexture(idx)); }
	inline const std::vector< ID3D11ShaderResourceView*>& GetTextureArray() const { return m_Texture->GetTextures(); }

	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& FontFileName, const tstring& TextureFileName);
	void Shutdown();
	void BuildVertexArray(const void* const& vertices, const tstring& sentence, const float& drawX, const float& drawY);

private:
	HRESULT LoadFontData(const tstring& FileName);
	HRESULT LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& FileName);
	void ReleaseFontData();
	void ReleaseTexture();

private:
	FontType* m_Font = nullptr;
	TextureClass* m_Texture = nullptr;
};

