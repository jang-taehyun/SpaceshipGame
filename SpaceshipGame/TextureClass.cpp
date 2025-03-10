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

	// 매개변수 검사 //
	if (_T("") == FileName)
	{
		return E_FAIL;
	}

	// 파일의 확장자 추출
	if (FileName.rfind('.') == std::string::npos)
	{
		return E_FAIL;
	}

	DotIdx = FileName.rfind('.');
	Extension = FileName.substr(DotIdx + (size_t)1);

	// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
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

	// 이미지 데이터에 맞는 SRV 생성 //
	return CreateShaderResourceView(Device, DeviceContext);
}

HRESULT TextureClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<tstring>& FileNames)
{
	tstring Extension;
	size_t DotIdx;

	for (int i = 0; i < FileNames.size(); ++i)
	{
		// 매개변수 검사 //
		if (_T("") == FileNames[i])
		{
			return E_FAIL;
		}

		// 파일의 확장자 추출
		if (FileNames[i].rfind('.') == std::string::npos)
		{
			return E_FAIL;
		}

		DotIdx = FileNames[i].rfind('.');
		Extension = FileNames[i].substr(DotIdx + (size_t)1);

		// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
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

		// 이미지 데이터에 맞는 SRV 생성 //
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
	// targa 파일을 binary 모드로 열기 //
	FILE* FilePtr = nullptr;
	std::string convert;
	convert.assign(FileName.begin(), FileName.end());
	if (fopen_s(&FilePtr, convert.c_str(), "rb"))
	{
		return E_FAIL;
	}

	// targa 파일의 header를 가져오기 //
	TargaHeader TargaFileHeader;
	unsigned int count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);
	if (1 != count)
	{
		return E_FAIL;
	}

	// targa 파일의 header에서 중요 정보 가져오기 //
	m_Height = TargaFileHeader.height;
	m_Width = TargaFileHeader.width;
	int bpp = TargaFileHeader.bpp;

	// targa 파일이 32 bit인지 24 bit인지 확인 //
	if (bpp != 32)
	{
		return E_FAIL;
	}

	// 32 bit 이미지 데이터의 크기 계산 //
	int ImageSize = m_Width * m_Height * 4;

	// targa 이미지 데이터용 메모리 할당 //
	unsigned char* TargaImage = nullptr;
	TargaImage = new unsigned char[ImageSize];
	if (!TargaImage)
	{
		return E_FAIL;
	}

	// targa 이미지 데이터 읽기 //
	count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);
	if (count != ImageSize)
	{
		return E_FAIL;
	}

	// targa 이미지 파일 닫기 //
	if (fclose(FilePtr))
	{
		return E_FAIL;
	}

	// targa 대상 데이터에 대한 메모리 할당 //
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		return E_FAIL;
	}

	// targa 대상 데이터 배열에 index 초기화 //
	int index = 0;

	// targa 이미지 데이터에 index 초기화 //
	// targa 이미지 데이터의 읽기 시작 위치
	// targa 데이터의 마지막 행에서 데이터의 읽기 시작
	int k = (m_Width * m_Height * 4) - (m_Width * 4);

	// targa 데이터를 정렬하여 메모리에 저장 //
	// targa 형식이 거꾸로 저장되었으므로 올바른 순서로 targa 이미지 데이터를 targa 대상 배열에 복사
	// 복사될 위치는 위에서 아래로, targa 이미지 데이터의 위치(복사할 위치)는 아래에서 위로 진행
	for (int j = 0; j < m_Height; j++)
	{
		// targa 데이터의 가로(width) 데이터를 메모리에 복사
		for (int i = 0; i < m_Width; i++)
		{
			// targa의 픽셀 데이터는 BGRA 순서로 저장되므로, 이를 RGBA 순서로 변환
			m_ImageData[index + 0] = TargaImage[k + 0];		// B
			m_ImageData[index + 1] = TargaImage[k + 1];		// G
			m_ImageData[index + 2] = TargaImage[k + 2];		// R
			m_ImageData[index + 3] = TargaImage[k + 3];		// A

			// index, 읽을 데이터의 시작 위치 증가 //
			k += 4;
			index += 4;
		}

		// targa 이미지 데이터 인덱스를 역순으로 읽은 후 열의 시작 부분에서 이전 행으로 다시 설정
		k -= (m_Width * 8);
	}

	// targa 이미지 데이터 해제 //
	delete[] TargaImage;
	TargaImage = nullptr;

	return S_OK;
}

HRESULT TextureClass::LoadPNG(const tstring& FileName)
{
	// 1. ScratchImage 생성
	DirectX::ScratchImage scratchImage;

	// 2. PNG 파일 로드 (WIC 기반)
	if (FAILED(DirectX::LoadFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratchImage)))
	{
		return E_FAIL;
	}

	// 3. 텍스처 크기 정보 추출
	const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);
	m_Width = static_cast<int>(image->width);
	m_Height = static_cast<int>(image->height);

	// 4. 데이터를 m_TargaData에 복사
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

	// 빈 texture 생성 //
	D3D11_TEXTURE2D_DESC TextureDesc;
	memset(&TextureDesc, 0, sizeof(TextureDesc));

	// texure 구조체 설정
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

	// 빈 texture 생성
	if (FAILED(Device->CreateTexture2D(&TextureDesc, NULL, &texture)))
	{
		return E_FAIL;
	}

	// 이미지 데이터를 빈 texture에 복사 //

	// 이미지 데이터의 width의 크기(바이트 크기) 구하기
	// 이미지는 RGBA 형식이므로, 한 pixel의 크기는 4byte
	UINT RowPitch = (m_Width * 4) * sizeof(unsigned char);

	// 이미지 데이터를 texture에 복사
	DeviceContext->UpdateSubresource(texture, 0, NULL, m_ImageData, RowPitch, 0);

	// shader resource view 생성 //
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	memset(&ShaderResourceViewDesc, 0, sizeof(ShaderResourceViewDesc));

	// shader resource view 설정
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view 생성
	if (FAILED(Device->CreateShaderResourceView(texture, &ShaderResourceViewDesc, &srv)))
	{
		return E_FAIL;
	}

	// texture의 Mipmap 생성 //
	DeviceContext->GenerateMips(srv);

	// 이미지 데이터 해제 //
	delete[] m_ImageData;
	m_ImageData = nullptr;

	// 생성된 texture, texture resource view를 멤버 변수에 저장 //
	m_Texture.push_back(texture);
	m_TextureView.push_back(srv);

	return S_OK;
}