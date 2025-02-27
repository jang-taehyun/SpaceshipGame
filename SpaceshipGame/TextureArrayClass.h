#pragma once

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, const TCHAR*, const TCHAR*);
	bool Initialize(ID3D11Device*, const TCHAR*, const TCHAR*, const TCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_Textures[3] = { 0, };
};

