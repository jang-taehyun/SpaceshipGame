#pragma once

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
	TextureClass(const TextureClass& other);
	~TextureClass();

	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::wstring& FileName);
	HRESULT Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& FileNames);
	void Shutdown();

	inline ID3D11ShaderResourceView* const& GetTexture(int idx) { return m_TextureView[idx]; }
	inline std::vector<ID3D11ShaderResourceView*> const& GetTextures() { return m_TextureView; }

private:
	HRESULT LoadTarga(const std::wstring& FileName);
	HRESULT LoadPNG(ID3D11Device* const& Device, const std::wstring& FileName);
	HRESULT LoadDDS(ID3D11Device* const& Device, const std::wstring& FileName);

	HRESULT CreateShaderResourceView(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext);

private:
	unsigned char* m_ImageData = nullptr;
	short m_Height = 0;
	short m_Width = 0;
	std::vector<ID3D11Texture2D*> m_Texture;
	std::vector<ID3D11ShaderResourceView*> m_TextureView;
};

