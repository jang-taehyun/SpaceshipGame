#include "pch.h"
#include <fstream>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "TextureClass.h"

Graphic::Texture::TextureClass::TextureClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName)
{
	HRESULT result = Initialize(Device, DeviceContext, FileName);
}

Graphic::Texture::TextureClass::TextureClass(const TextureClass& other)
{
	other.m_Texture.CopyTo(m_Texture.GetAddressOf());
}

Graphic::Texture::TextureClass::TextureClass(TextureClass&& other) noexcept : m_Texture(std::move(other.m_Texture)) {}

Graphic::Texture::TextureClass& Graphic::Texture::TextureClass::operator=(const TextureClass& other)
{
	if (this == &other)
		return *this;

	other.m_Texture.CopyTo(this->m_Texture.ReleaseAndGetAddressOf());

	return *this;
}

Graphic::Texture::TextureClass& Graphic::Texture::TextureClass::operator=(TextureClass&& other) noexcept
{
	if (this == &other)
		return *this;

	this->m_Texture = std::move(other.m_Texture);

	return *this;
}

HRESULT Graphic::Texture::TextureClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName)
{
	HRESULT result = S_OK;
	std::wstring Extension;
	size_t DotIdx = 0;

	// 매개변수 검사 //
	assert(_T("") != FileName);
	assert(FileName.rfind('.') != std::string::npos);

	// 파일의 확장자 추출
	DotIdx = FileName.rfind('.');
	Extension = FileName.substr(DotIdx + (size_t)1);

	// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
	return Load(Device, DeviceContext, FileName, Extension);
}

HRESULT Graphic::Texture::TextureClass::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName, const std::wstring& Extension)
{
	HRESULT result = S_OK;

	// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
	if (_T("png") == Extension || _T("PNG") == Extension || _T("jpg") == Extension || _T("JPG") == Extension)
		result = LoadWIC(Device, FileName);
	else if (_T("tga") == Extension || _T("TGA") == Extension)
		result = LoadTarga(Device, DeviceContext, FileName);
	else if (_T("dds") == Extension || _T("DDS") == Extension)
		result = LoadDDS(Device, FileName);
	else
		assert(false);

	return result;
}

HRESULT Graphic::Texture::TextureClass::LoadTarga(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName)
{
	HRESULT result = S_OK;
	std::unique_ptr<char> image = nullptr;
	UINT width = 0, height = 0;

	// TGA 파일을 메모리에 load //
	image = std::move(LoadTargaFile(FileName, height, width));
	assert(image);

	// 메모리에 load한 TGA 파일을 이용해 shader resource view 생성 //
	result = CreateShaderResourceView(Device, DeviceContext, image.get(), height, width);

	return result;
}

HRESULT Graphic::Texture::TextureClass::LoadWIC(ID3D11Device* Device, const std::wstring& FileName)
{
	HRESULT result = S_OK;
	
	result = DirectX::CreateWICTextureFromFile(Device, FileName.c_str(), nullptr, m_Texture.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

HRESULT Graphic::Texture::TextureClass::LoadDDS(ID3D11Device* Device, const std::wstring& FileName)
{
	HRESULT result = S_OK;

	result = DirectX::CreateDDSTextureFromFile(Device, FileName.c_str(), nullptr, m_Texture.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

std::unique_ptr<char> Graphic::Texture::TextureClass::LoadTargaFile(const std::wstring& FileName, UINT& Height, UINT& Width)
{
	HRESULT result = S_OK;
	std::ifstream FileIn;
	TargaHeader TargaFileHeader = {};				// targa 파일의 header 정보
	int bpp = 0;									// targa 파일의 색상 bit 수(32bit 또는 24bit)
	int ImageSize = 0;								// targa 이미지의 크기
	std::unique_ptr<char> TargaImage = nullptr;		// targa 이미지 데이터

	// targa 파일을 binary 모드로 열기 //
	FileIn.open(FileName, std::ios::in | std::ios::binary);
	assert(FileIn.is_open());

	// targa 파일의 header를 가져오기 //
	FileIn.read(reinterpret_cast<char*>(&TargaFileHeader), sizeof(TargaFileHeader));
	assert(static_cast<unsigned int>(FileIn.gcount()) == sizeof(TargaFileHeader));

	Height = TargaFileHeader.height;
	Width = TargaFileHeader.width;

	// targa 파일이 32 bit인지 24 bit인지 확인 //
	assert(32 != TargaFileHeader.bpp);

	// 32 bit 이미지 데이터의 크기 계산 //
	ImageSize = TargaFileHeader.width * TargaFileHeader.height * 4;

	// targa 이미지 데이터용 메모리 할당 및 targa 이미지 데이터 읽기 //
	TargaImage = std::make_unique<char>(ImageSize);
	FileIn.read(TargaImage.get(), ImageSize);
	assert(static_cast<unsigned int>(FileIn.gcount()) == ImageSize);

	// targa 이미지 파일 닫기 //
	FileIn.close();

	return std::move(TargaImage);
}

HRESULT Graphic::Texture::TextureClass::CreateShaderResourceView(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, char* ImageData, UINT Height, UINT Width)
{
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC TextureDesc = {};							// texture 설정 정보
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc = {};	// shader resoure view 설정 정보
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;		// texture
	UINT RowPitch = 0;
	
	// 빈 texture 생성 //
	// texure 구조체 설정
	TextureDesc.Height = Height;
	TextureDesc.Width = Width;
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
	result = Device->CreateTexture2D(&TextureDesc, NULL, texture.GetAddressOf());
	assert(SUCCEEDED(result));

	// 이미지 데이터를 빈 texture에 복사 //

	// 이미지 데이터의 width의 크기(바이트 크기) 구하기
	// 이미지는 RGBA 형식이므로, 한 pixel의 크기는 4byte
	RowPitch = (Width * 4) * sizeof(char);

	// 이미지 데이터를 texture에 복사
	DeviceContext->UpdateSubresource(texture.Get(), 0, NULL, ImageData, RowPitch, 0);

	// shader resource view 생성 //
	// shader resource view 설정
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view 생성
	result = Device->CreateShaderResourceView(texture.Get(), &ShaderResourceViewDesc, m_Texture.GetAddressOf());
	assert(SUCCEEDED(result));

	// texture의 Mipmap 생성 //
	DeviceContext->GenerateMips(m_Texture.Get());

	return result;
}