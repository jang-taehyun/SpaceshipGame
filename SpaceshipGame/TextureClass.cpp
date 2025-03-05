#include "pch.h"
#include <cstdio>
#include "TextureClass.h"

TextureClass::TextureClass() {}
TextureClass::TextureClass(const TextureClass& other) {}
TextureClass::~TextureClass() {}

bool TextureClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	int width = 0, height = 0;

	// targa 이미지 데이터를 메모리에 로드 //
	if (FileName && !LoadTarga(FileName, height, width))
	{
		return false;
	}

	// 이미지 데이터에 맞는 SRV 생성 //
	if (!CreateShaderResourceView(Device, DeviceContext, height, width, true))
	{
		return false;
	}
	
	return true;
}

bool TextureClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tFileName)
{
	int width = 0, height = 0;

	// PNG 이미지 데이터를 메모리에 로드 //
	if (tFileName && !LoadPNG(tFileName, height, width))
	{
		return false;
	}

	// 이미지 데이터에 맞는 SRV 생성 //
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
	// targa 파일을 binary 모드로 열기 //
	FILE* FilePtr = nullptr;
	if (fopen_s(&FilePtr, FileName, "rb"))
	{
		return false;
	}

	// targa 파일의 header를 가져오기 //
	TargaHeader TargaFileHeader;
	unsigned int count = (unsigned int)fread(&TargaFileHeader, sizeof(TargaHeader), 1, FilePtr);
	if (1 != count)
	{
		return false;
	}

	// targa 파일의 header에서 중요 정보 가져오기 //
	Height = (int)TargaFileHeader.height;
	Width = (int)TargaFileHeader.width;
	int bpp = (int)TargaFileHeader.bpp;

	// targa 파일이 32 bit인지 24 bit인지 확인 //
	if (bpp != 32)
	{
		return false;
	}

	// 32 bit 이미지 데이터의 크기 계산 //
	int ImageSize = Width * Height * 4;

	// targa 이미지 데이터용 메모리 할당 //
	unsigned char* TargaImage = nullptr;
	TargaImage = new unsigned char[ImageSize];
	if (!TargaImage)
	{
		return false;
	}

	// targa 이미지 데이터 읽기 //
	count = (unsigned int)fread(TargaImage, 1, ImageSize, FilePtr);
	if (count != ImageSize)
	{
		return false;
	}

	// targa 이미지 파일 닫기 //
	if (fclose(FilePtr))
	{
		return false;
	}

	// targa 대상 데이터에 대한 메모리 할당 //
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		return false;
	}

	// targa 대상 데이터 배열에 index 초기화 //
	int index = 0;

	// targa 이미지 데이터에 index 초기화 //
	// targa 이미지 데이터의 읽기 시작 위치
	// targa 데이터의 마지막 행에서 데이터의 읽기 시작
	int k = (Width * Height * 4) - (Width * 4);

	// targa 데이터를 정렬하여 메모리에 저장 //
	// targa 형식이 거꾸로 저장되었으므로 올바른 순서로 targa 이미지 데이터를 targa 대상 배열에 복사
	// 복사될 위치는 위에서 아래로, targa 이미지 데이터의 위치(복사할 위치)는 아래에서 위로 진행
	for (int j = 0; j < Height; j++)
	{
		// targa 데이터의 가로(width) 데이터를 메모리에 복사
		for (int i = 0; i < Width; i++)
		{
			// targa의 픽셀 데이터는 BGRA 순서로 저장되므로, 이를 RGBA 순서로 변환
			m_ImageData[index + 0] = TargaImage[k + 2];		// R
			m_ImageData[index + 1] = TargaImage[k + 1];		// G
			m_ImageData[index + 2] = TargaImage[k + 0];		// B
			m_ImageData[index + 3] = TargaImage[k + 3];		// A

			// index, 읽을 데이터의 시작 위치 증가 //
			k += 4;
			index += 4;
		}

		// targa 이미지 데이터 인덱스를 역순으로 읽은 후 열의 시작 부분에서 이전 행으로 다시 설정
		k -= (Width * 8);
	}

	// targa 이미지 데이터 해제 //
	delete[] TargaImage;
	TargaImage = nullptr;

	return true;
}

bool TextureClass::LoadPNG(const TCHAR* FileName, int& Height, int& Width)
{
	// 1. ScratchImage 생성
	DirectX::ScratchImage scratchImage;

	// 2. PNG 파일 로드 (WIC 기반)
	if (FAILED(DirectX::LoadFromWICFile(FileName, DirectX::WIC_FLAGS_NONE, nullptr, scratchImage)))
	{
		return false;
	}

	// 3. 텍스처 크기 정보 추출
	const DirectX::Image* image = scratchImage.GetImage(0, 0, 0);
	Width = static_cast<int>(image->width);
	Height = static_cast<int>(image->height);

	// 4. 데이터를 m_TargaData에 복사
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
	// 빈 texture 생성 //
	D3D11_TEXTURE2D_DESC TextureDesc;
	memset(&TextureDesc, 0, sizeof(TextureDesc));

	// texure 구조체 설정
	TextureDesc.Height = Height;
	TextureDesc.Width = Width;
	TextureDesc.MipLevels = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = (IsTarga ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM);		// -> targa 파일을 사용하면 DXGI_FORMAT_R8G8B8A8_UNORM, PNG 파일을 사용하면 DXGI_FORMAT_B8G8R8A8_UNORM
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// 빈 texture 생성
	if (FAILED(Device->CreateTexture2D(&TextureDesc, NULL, &m_Texture)))
	{
		return false;
	}

	// 이미지 데이터를 빈 texture에 복사 //

	// 이미지 데이터의 width의 크기(바이트 크기) 구하기
	// 이미지는 RGBA 형식이므로, 한 pixel의 크기는 4byte
	UINT RowPitch = (Width * 4) * sizeof(unsigned char);

	// 이미지 데이터를 texture에 복사
	DeviceContext->UpdateSubresource(m_Texture, 0, NULL, m_ImageData, RowPitch, 0);

	// shader resource view 생성 //
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	memset(&ShaderResourceViewDesc, 0, sizeof(ShaderResourceViewDesc));

	// shader resource view 설정
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view 생성
	if (FAILED(Device->CreateShaderResourceView(m_Texture, &ShaderResourceViewDesc, &m_TextureView)))
	{
		return false;
	}

	// texture의 Mipmap 생성 //
	DeviceContext->GenerateMips(m_TextureView);

	// 이미지 데이터 해제 //
	delete[] m_ImageData;
	m_ImageData = nullptr;

	return true;
}