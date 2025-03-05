#include "pch.h"
#include <cstdio>
#include "TextureClass.h"

TextureClass::TextureClass() {}
TextureClass::TextureClass(const TextureClass& other) {}
TextureClass::~TextureClass() {}

bool TextureClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	int width = 0, height = 0;

	// targa �̹��� �����͸� �޸𸮿� �ε� //
	if (FileName && !LoadTarga(FileName, height, width))
	{
		return false;
	}

	// �̹��� �����Ϳ� �´� SRV ���� //
	if (!CreateShaderResourceView(Device, DeviceContext, height, width, true))
	{
		return false;
	}
	
	return true;
}

bool TextureClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tFileName)
{
	int width = 0, height = 0;

	// PNG �̹��� �����͸� �޸𸮿� �ε� //
	if (tFileName && !LoadPNG(tFileName, height, width))
	{
		return false;
	}

	// �̹��� �����Ϳ� �´� SRV ���� //
	if (!CreateShaderResourceView(Device, DeviceContext, height, width, false))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	if (m_TextureView)
	{
		m_TextureView->Release();
		m_TextureView = nullptr;
	}

	if (m_Texture)
	{
		m_Texture->Release();
		m_Texture = nullptr;
	}

	if (m_ImageData)
	{
		delete[] m_ImageData;
		m_ImageData = nullptr;
	}
}

bool TextureClass::LoadTarga(const char* FileName, int& Height, int& Width)
{
	// targa ������ binary ���� ���� //
	FILE* FilePtr = nullptr;
	if (fopen_s(&FilePtr, FileName, "rb"))
	{
		return false;
	}

	// targa ������ header�� �������� //
	TargaHeader TargaFileHeader;
	unsigned int count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);
	if (1 != count)
	{
		return false;
	}

	// targa ������ header���� �߿� ���� �������� //
	Height = (int)TargaFileHeader.height;
	Width = (int)TargaFileHeader.width;
	int bpp = (int)TargaFileHeader.bpp;

	// targa ������ 32 bit���� 24 bit���� Ȯ�� //
	if (bpp != 32)
	{
		return false;
	}

	// 32 bit �̹��� �������� ũ�� ��� //
	int ImageSize = Width * Height * 4;

	// targa �̹��� �����Ϳ� �޸� �Ҵ� //
	unsigned char* TargaImage = nullptr;
	TargaImage = new unsigned char[ImageSize];
	if (!TargaImage)
	{
		return false;
	}

	// targa �̹��� ������ �б� //
	count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);
	if (count != ImageSize)
	{
		return false;
	}

	// targa �̹��� ���� �ݱ� //
	if (fclose(FilePtr))
	{
		return false;
	}

	// targa ��� �����Ϳ� ���� �޸� �Ҵ� //
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		return false;
	}

	// targa ��� ������ �迭�� index �ʱ�ȭ //
	int index = 0;

	// targa �̹��� �����Ϳ� index �ʱ�ȭ //
	// targa �̹��� �������� �б� ���� ��ġ
	// targa �������� ������ �࿡�� �������� �б� ����
	int k = (Width * Height * 4) - (Width * 4);

	// targa �����͸� �����Ͽ� �޸𸮿� ���� //
	// targa ������ �Ųٷ� ����Ǿ����Ƿ� �ùٸ� ������ targa �̹��� �����͸� targa ��� �迭�� ����
	// ����� ��ġ�� ������ �Ʒ���, targa �̹��� �������� ��ġ(������ ��ġ)�� �Ʒ����� ���� ����
	for (int j = 0; j < Height; j++)
	{
		// targa �������� ����(width) �����͸� �޸𸮿� ����
		for (int i = 0; i < Width; i++)
		{
			// targa�� �ȼ� �����ʹ� BGRA ������ ����ǹǷ�, �̸� RGBA ������ ��ȯ
			m_ImageData[index + 0] = TargaImage[k + 2];		// R
			m_ImageData[index + 1] = TargaImage[k + 1];		// G
			m_ImageData[index + 2] = TargaImage[k + 0];		// B
			m_ImageData[index + 3] = TargaImage[k + 3];		// A

			// index, ���� �������� ���� ��ġ ���� //
			k += 4;
			index += 4;
		}

		// targa �̹��� ������ �ε����� �������� ���� �� ���� ���� �κп��� ���� ������ �ٽ� ����
		k -= (Width * 8);
	}

	// targa �̹��� ������ ���� //
	delete[] TargaImage;
	TargaImage = nullptr;

	return true;
}

bool TextureClass::LoadPNG(const TCHAR* FileName, int& Height, int& Width)
{
	// 1. ScratchImage ����
	DirectX::ScratchImage scratchImage;

	// 2. PNG ���� �ε� (WIC ���)
	if (FAILED(DirectX::LoadFromWICFile(FileName, DirectX::WIC_FLAGS_NONE, nullptr, scratchImage)))
	{
		return false;
	}

	// 3. �ؽ�ó ũ�� ���� ����
	const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);
	Width = static_cast<int>(image->width);
	Height = static_cast<int>(image->height);

	// 4. �����͸� m_TargaData�� ����
	int ImageSize = Width * Height * 4;
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		return false;
	}
	memcpy(m_ImageData, image->pixels, ImageSize);

	return true;
}

bool TextureClass::CreateShaderResourceView(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, int& Height, int& Width, bool IsTarga)
{
	// �� texture ���� //
	D3D11_TEXTURE2D_DESC TextureDesc;
	memset(&TextureDesc, 0, sizeof(TextureDesc));

	// texure ����ü ����
	TextureDesc.Height = Height;
	TextureDesc.Width = Width;
	TextureDesc.MipLevels = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = (IsTarga ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM);		// -> targa ������ ����ϸ� DXGI_FORMAT_R8G8B8A8_UNORM, PNG ������ ����ϸ� DXGI_FORMAT_B8G8R8A8_UNORM
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// �� texture ����
	if (FAILED(Device->CreateTexture2D(&TextureDesc, NULL, &m_Texture)))
	{
		return false;
	}

	// �̹��� �����͸� �� texture�� ���� //

	// �̹��� �������� width�� ũ��(����Ʈ ũ��) ���ϱ�
	// �̹����� RGBA �����̹Ƿ�, �� pixel�� ũ��� 4byte
	UINT RowPitch = (Width * 4) * sizeof(unsigned char);

	// �̹��� �����͸� texture�� ����
	DeviceContext->UpdateSubresource(m_Texture, 0, NULL, m_ImageData, RowPitch, 0);

	// shader resource view ���� //
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	memset(&ShaderResourceViewDesc, 0, sizeof(ShaderResourceViewDesc));

	// shader resource view ����
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view ����
	if (FAILED(Device->CreateShaderResourceView(m_Texture, &ShaderResourceViewDesc, &m_TextureView)))
	{
		return false;
	}

	// texture�� Mipmap ���� //
	DeviceContext->GenerateMips(m_TextureView);

	// �̹��� ������ ���� //
	delete[] m_ImageData;
	m_ImageData = nullptr;

	return true;
}