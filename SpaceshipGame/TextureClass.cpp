#include "pch.h"
#include "DDSTextureLoader.h"
#include "TextureClass.h"

TextureClass::TextureClass() {}
TextureClass::TextureClass(const TextureClass& other) {}
TextureClass::~TextureClass() {}

HRESULT TextureClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& FileName)
{
	tstring Extension;
	size_t DotIdx;

	// �Ű����� �˻� //
	if (_T("") == FileName)
	{
		return E_FAIL;
	}

	// ������ Ȯ���� ����
	if (FileName.rfind('.') == std::string::npos)
	{
		return E_FAIL;
	}

	DotIdx = FileName.rfind('.');
	Extension = FileName.substr(DotIdx + (size_t)1);

	// ������ Ȯ���ڿ� ���� �̹��� �ε� �Լ��� ȣ���� �޸𸮿� �̹��� ������ �ε�
	if (_T("png") == Extension || _T("PNG") == Extension || _T("jpg") == Extension || _T("JPG") == Extension)
	{
		if (FAILED(LoadPNG(FileName)))
			return E_FAIL;
	}
	else if (_T("tga") == Extension || _T("TGA") == Extension)
	{
		if (FAILED(LoadTarga(FileName)))
			return E_FAIL;
	}
	else if (_T("dds") == Extension || _T("DDS") == Extension)
	{
		if (FAILED(LoadDDS(Device, FileName)))
			return E_FAIL;
		else
			return S_OK;
	}

	// �̹��� �����Ϳ� �´� SRV ���� //
	return CreateShaderResourceView(Device, DeviceContext);
}

HRESULT TextureClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<tstring>& FileNames)
{
	tstring Extension;
	size_t DotIdx;

	for (int i = 0; i < FileNames.size(); ++i)
	{
		// �Ű����� �˻� //
		if (_T("") == FileNames[i])
		{
			return E_FAIL;
		}

		// ������ Ȯ���� ����
		if (FileNames[i].rfind('.') == std::string::npos)
		{
			return E_FAIL;
		}

		DotIdx = FileNames[i].rfind('.');
		Extension = FileNames[i].substr(DotIdx + (size_t)1);

		// ������ Ȯ���ڿ� ���� �̹��� �ε� �Լ��� ȣ���� �޸𸮿� �̹��� ������ �ε�
		if (_T("png") == Extension || _T("PNG") == Extension || _T("jpg") == Extension || _T("JPG") == Extension)
		{
			if (FAILED(LoadPNG(FileNames[i])))
				return E_FAIL;
		}
		else if (_T("tga") == Extension || _T("TGA") == Extension)
		{
			if (FAILED(LoadTarga(FileNames[i])))
				return E_FAIL;
		}
		else if (_T("dds") == Extension || _T("DDS") == Extension)
		{
			if (FAILED(LoadDDS(Device, FileNames[i])))
				return E_FAIL;
			else
				return S_OK;
		}

		// �̹��� �����Ϳ� �´� SRV ���� //
		if(FAILED(CreateShaderResourceView(Device, DeviceContext)))
			return E_FAIL;
	}

	return S_OK;
}

void TextureClass::Shutdown()
{
	if (!m_TextureView.empty())
	{
		for (int i = 0; i < m_TextureView.size(); ++i)
		{
			if (!m_TextureView[i])
			{
				m_TextureView[i]->Release();
				m_TextureView[i] = nullptr;
			}
		}

		m_TextureView.clear();
	}

	if (!m_Texture.empty())
	{
		for (int i = 0; i < m_Texture.size(); ++i)
		{
			if (!m_Texture[i])
			{
				m_Texture[i]->Release();
				m_Texture[i] = nullptr;
			}
		}

		m_Texture.clear();
	}

	if (m_ImageData)
	{
		delete[] m_ImageData;
		m_ImageData = nullptr;
	}
}

HRESULT TextureClass::LoadTarga(const tstring& FileName)
{
	// targa ������ binary ���� ���� //
	FILE* FilePtr = nullptr;
	std::string convert;
	convert.assign(FileName.begin(), FileName.end());
	if (fopen_s(&FilePtr, convert.c_str(), "rb"))
	{
		return E_FAIL;
	}

	// targa ������ header�� �������� //
	TargaHeader TargaFileHeader;
	unsigned int count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);
	if (1 != count)
	{
		return E_FAIL;
	}

	// targa ������ header���� �߿� ���� �������� //
	m_Height = TargaFileHeader.height;
	m_Width = TargaFileHeader.width;
	int bpp = TargaFileHeader.bpp;

	// targa ������ 32 bit���� 24 bit���� Ȯ�� //
	if (bpp != 32)
	{
		return E_FAIL;
	}

	// 32 bit �̹��� �������� ũ�� ��� //
	int ImageSize = m_Width * m_Height * 4;

	// targa �̹��� �����Ϳ� �޸� �Ҵ� //
	unsigned char* TargaImage = nullptr;
	TargaImage = new unsigned char[ImageSize];
	if (!TargaImage)
	{
		return E_FAIL;
	}

	// targa �̹��� ������ �б� //
	count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);
	if (count != ImageSize)
	{
		return E_FAIL;
	}

	// targa �̹��� ���� �ݱ� //
	if (fclose(FilePtr))
	{
		return E_FAIL;
	}

	// targa ��� �����Ϳ� ���� �޸� �Ҵ� //
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		return E_FAIL;
	}

	// targa ��� ������ �迭�� index �ʱ�ȭ //
	int index = 0;

	// targa �̹��� �����Ϳ� index �ʱ�ȭ //
	// targa �̹��� �������� �б� ���� ��ġ
	// targa �������� ������ �࿡�� �������� �б� ����
	int k = (m_Width * m_Height * 4) - (m_Width * 4);

	// targa �����͸� �����Ͽ� �޸𸮿� ���� //
	// targa ������ �Ųٷ� ����Ǿ����Ƿ� �ùٸ� ������ targa �̹��� �����͸� targa ��� �迭�� ����
	// ����� ��ġ�� ������ �Ʒ���, targa �̹��� �������� ��ġ(������ ��ġ)�� �Ʒ����� ���� ����
	for (int j = 0; j < m_Height; j++)
	{
		// targa �������� ����(width) �����͸� �޸𸮿� ����
		for (int i = 0; i < m_Width; i++)
		{
			// targa�� �ȼ� �����ʹ� BGRA ������ ����ǹǷ�, �̸� RGBA ������ ��ȯ
			m_ImageData[index + 0] = TargaImage[k + 0];		// B
			m_ImageData[index + 1] = TargaImage[k + 1];		// G
			m_ImageData[index + 2] = TargaImage[k + 2];		// R
			m_ImageData[index + 3] = TargaImage[k + 3];		// A

			// index, ���� �������� ���� ��ġ ���� //
			k += 4;
			index += 4;
		}

		// targa �̹��� ������ �ε����� �������� ���� �� ���� ���� �κп��� ���� ������ �ٽ� ����
		k -= (m_Width * 8);
	}

	// targa �̹��� ������ ���� //
	delete[] TargaImage;
	TargaImage = nullptr;

	return S_OK;
}

HRESULT TextureClass::LoadPNG(const tstring& FileName)
{
	// 1. ScratchImage ����
	DirectX::ScratchImage scratchImage;

	// 2. PNG ���� �ε� (WIC ���)
	if (FAILED(DirectX::LoadFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratchImage)))
	{
		return E_FAIL;
	}

	// 3. �ؽ�ó ũ�� ���� ����
	const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);
	m_Width = static_cast<int>(image->width);
	m_Height = static_cast<int>(image->height);

	// 4. �����͸� m_TargaData�� ����
	int ImageSize = m_Width * m_Height * 4;
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		return E_FAIL;
	}
	memcpy(m_ImageData, image->pixels, ImageSize);

	return S_OK;
}

HRESULT TextureClass::LoadDDS(ID3D11Device* const& Device, const tstring& FileName)
{
	ID3D11ShaderResourceView* srv;
	if (FAILED(DirectX::CreateDDSTextureFromFile(Device, FileName.c_str(), nullptr, &srv)))
		return E_FAIL;

	m_TextureView.push_back(srv);

	return S_OK;
}

HRESULT TextureClass::CreateShaderResourceView(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

	// �� texture ���� //
	D3D11_TEXTURE2D_DESC TextureDesc;
	memset(&TextureDesc, 0, sizeof(TextureDesc));

	// texure ����ü ����
	TextureDesc.Height = m_Height;
	TextureDesc.Width = m_Width;
	TextureDesc.MipLevels = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// �� texture ����
	if (FAILED(Device->CreateTexture2D(&TextureDesc, NULL, &texture)))
	{
		return E_FAIL;
	}

	// �̹��� �����͸� �� texture�� ���� //

	// �̹��� �������� width�� ũ��(����Ʈ ũ��) ���ϱ�
	// �̹����� RGBA �����̹Ƿ�, �� pixel�� ũ��� 4byte
	UINT RowPitch = (m_Width * 4) * sizeof(unsigned char);

	// �̹��� �����͸� texture�� ����
	DeviceContext->UpdateSubresource(texture, 0, NULL, m_ImageData, RowPitch, 0);

	// shader resource view ���� //
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	memset(&ShaderResourceViewDesc, 0, sizeof(ShaderResourceViewDesc));

	// shader resource view ����
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view ����
	if (FAILED(Device->CreateShaderResourceView(texture, &ShaderResourceViewDesc, &srv)))
	{
		return E_FAIL;
	}

	// texture�� Mipmap ���� //
	DeviceContext->GenerateMips(srv);

	// �̹��� ������ ���� //
	delete[] m_ImageData;
	m_ImageData = nullptr;

	// ������ texture, texture resource view�� ��� ������ ���� //
	m_Texture.push_back(texture);
	m_TextureView.push_back(srv);

	return S_OK;
}