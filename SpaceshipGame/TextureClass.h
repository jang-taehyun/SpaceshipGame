#pragma once

class Image;

class TextureClass
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const char*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, const TCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(const char*, int&, int&);
	bool LoadPNG(const TCHAR*, int&, int&);

	bool CreateShaderResourceView(ID3D11Device*, ID3D11DeviceContext*, int&, int&, bool);

private:
	unsigned char* m_ImageData = nullptr;
	ID3D11Texture2D* m_Texture = nullptr;
	ID3D11ShaderResourceView* m_TextureView = nullptr;
};

