#pragma once

namespace Graphic
{
	namespace Texture
	{
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
			TextureClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UITextureID ID);
			TextureClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& filename);
			TextureClass(const TextureClass& other);
			TextureClass(TextureClass&& other) noexcept;
			virtual ~TextureClass() = default;

			ID3D11ShaderResourceView* GetTexture() const { return m_Texture.Get(); }
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> MoveTexture() { return std::move(m_Texture); }
			D3D11_TEXTURE2D_DESC GetTextureInfo() const { return m_TextureInfo; }

			TextureClass& operator=(const TextureClass& other);
			TextureClass& operator=(TextureClass&& other) noexcept;

		private:
			HRESULT Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName);

			HRESULT Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName, const std::wstring& Extension);

			HRESULT LoadTarga(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName);
			HRESULT LoadWIC(ID3D11Device* Device, const std::wstring& FileName);
			HRESULT LoadDDS(ID3D11Device* Device, const std::wstring& FileName);

			std::unique_ptr<char> LoadTargaFile(const std::wstring& FileName, UINT& Height, UINT& Width);
			HRESULT CreateShaderResourceView(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, char* ImageData, UINT Height, UINT Width);

		private:
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Texture = nullptr;
			D3D11_TEXTURE2D_DESC m_TextureInfo = {};
		};
	}
}
