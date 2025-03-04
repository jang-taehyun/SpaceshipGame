#pragma once

class TextureClass;

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*, const char*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*, const TCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, const char*, float, float);

private:
	bool LoadFontData(const char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*);
	void ReleaseTexture();

private:
	FontType* m_Font = nullptr;
	TextureClass* m_Texture = nullptr;
};

