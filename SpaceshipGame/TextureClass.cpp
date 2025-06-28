#include "pch.h"
#include <fstream>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "TextureClass.h"

static ErrorContent e;

Graphic::Texture::TextureClass::TextureClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass Constructor");

	result = Initialize(Device, DeviceContext, FileName);
	if (FAILED(result))
		throw e;
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
	size_t DotIdx;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass Initialize()");

	// 매개변수 검사 //
	if (_T("") == FileName)
	{
		e.contents = _T("필요한 파일 정보(경로, 이름)이 없습니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// 파일의 확장자 추출
	if (FileName.rfind('.') == std::string::npos)
	{
		e.contents = _T("필요한 파일 정보(경로, 이름)에 확장자가 없습니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}
	DotIdx = FileName.rfind('.');
	Extension = FileName.substr(DotIdx + (size_t)1);

	// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
	return Load(Device, DeviceContext, FileName, Extension);
}

HRESULT Graphic::Texture::TextureClass::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName, const std::wstring& Extension)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass Load()");

	// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
	if (_T("png") == Extension || _T("PNG") == Extension || _T("jpg") == Extension || _T("JPG") == Extension)
	{
		result = LoadWIC(Device, FileName);
		if (FAILED(result))
			return result;
	}
	else if (_T("tga") == Extension || _T("TGA") == Extension)
	{
		result = LoadTarga(Device, DeviceContext, FileName);
		if (FAILED(result))
			return result;
	}
	else if (_T("dds") == Extension || _T("DDS") == Extension)
	{
		result = LoadDDS(Device, FileName);
		if (FAILED(result))
			return result;
	}
	else
	{
		e.contents = _T("지원하지 않는 확장자입니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

HRESULT Graphic::Texture::TextureClass::LoadTarga(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::wstring& FileName)
{
	HRESULT result = S_OK;
	std::unique_ptr<char> image;
	UINT width = 0, height = 0;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadTarga()");

	// TGA 파일을 메모리에 load //
	image = std::move(LoadTargaFile(FileName, height, width));
	if (!image)
		return E_FAIL;

	// 메모리에 load한 TGA 파일을 이용해 shader resource view 생성 //
	result = CreateShaderResourceView(Device, DeviceContext, std::move(image), height, width);

	return result;
}

HRESULT Graphic::Texture::TextureClass::LoadWIC(ID3D11Device* Device, const std::wstring& FileName)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadPNG()");
	
	result = DirectX::CreateWICTextureFromFile(Device, FileName.c_str(), nullptr, m_Texture.GetAddressOf());
	if (FAILED(result))
	{
		e.contents = _T("WIC texture 파일(png, jpg 등) load 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

HRESULT Graphic::Texture::TextureClass::LoadDDS(ID3D11Device* Device, const std::wstring& FileName)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadDDS()");

	result = DirectX::CreateDDSTextureFromFile(Device, FileName.c_str(), nullptr, m_Texture.GetAddressOf());
	if (FAILED(result))
	{
		e.contents = _T("DDS texture 파일 load 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

std::unique_ptr<char> Graphic::Texture::TextureClass::LoadTargaFile(const std::wstring& FileName, UINT& Height, UINT& Width)
{
	HRESULT result = S_OK;
	std::ifstream FileIn;
	TargaHeader TargaFileHeader;		// targa 파일의 header 정보
	int bpp = 0;						// targa 파일의 색상 bit 수(32bit 또는 24bit)
	int ImageSize = 0;					// targa 이미지의 크기
	std::unique_ptr<char> TargaImage;	// targa 이미지 데이터

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadTargaFile()");

	// targa 파일을 binary 모드로 열기 //
	FileIn.open(FileName, std::ios::in | std::ios::binary);
	if (FileIn.fail())
	{
		e.contents = _T("TGA 파일 열기 실패");
		e.errorCode = E_FAIL;
		return nullptr;
	}

	// targa 파일의 header를 가져오기 //
	FileIn.read(reinterpret_cast<char*>(&TargaFileHeader), sizeof(TargaFileHeader));
	if (static_cast<unsigned int>(FileIn.gcount()) != sizeof(TargaFileHeader))
	{
		FileIn.close();
		e.contents = _T("TGA 파일의 header를 가져오기 실패");
		e.errorCode = E_FAIL;
		return nullptr;
	}

	// targa 파일의 header에서 중요 정보 가져오기 //
	Height = TargaFileHeader.height;
	Width = TargaFileHeader.width;
	bpp = TargaFileHeader.bpp;

	// targa 파일이 32 bit인지 24 bit인지 확인 //
	if (32 != bpp)
	{
		FileIn.close();
		e.contents = _T("targa 파일이 32bit가 아닙니다.");
		e.errorCode = E_FAIL;
		return nullptr;
	}

	// 32 bit 이미지 데이터의 크기 계산 //
	ImageSize = Width * Height * 4;

	// targa 이미지 데이터용 메모리 할당 및 targa 이미지 데이터 읽기 //
	TargaImage = std::make_unique<char>(ImageSize);
	FileIn.read(TargaImage.get(), ImageSize);
	if (static_cast<unsigned int>(FileIn.gcount()) != ImageSize)
	{
		FileIn.close();
		e.contents = _T("targa 이미지 데이터용 메모리 할당 및 targa 이미지 데이터 읽기 실패");
		e.errorCode = E_FAIL;
		return nullptr;
	}

	// targa 이미지 파일 닫기 //
	FileIn.close();

	return std::move(TargaImage);
}

HRESULT Graphic::Texture::TextureClass::CreateShaderResourceView(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::unique_ptr<char> ImageData, UINT Height, UINT Width)
{
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC TextureDesc;							// texture 설정 정보
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;		// shader resoure view 설정 정보
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;	// texture
	UINT RowPitch = 0;

	// 에러 메세지, 구조체 초기화 //
	e.title = _T("TextureClass CreateShaderResourceView()");
	memset(&TextureDesc, 0, sizeof(TextureDesc));
	memset(&ShaderResourceViewDesc, 0, sizeof(ShaderResourceViewDesc));
	
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
	if (FAILED(result))
	{
		e.contents = _T("빈 texture 생성 실패");
		e.errorCode = result;
		return result;
	}

	// 이미지 데이터를 빈 texture에 복사 //

	// 이미지 데이터의 width의 크기(바이트 크기) 구하기
	// 이미지는 RGBA 형식이므로, 한 pixel의 크기는 4byte
	RowPitch = (Width * 4) * sizeof(char);

	// 이미지 데이터를 texture에 복사
	DeviceContext->UpdateSubresource(texture.Get(), 0, NULL, ImageData.get(), RowPitch, 0);

	// shader resource view 생성 //
	// shader resource view 설정
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view 생성
	result = Device->CreateShaderResourceView(texture.Get(), &ShaderResourceViewDesc, m_Texture.GetAddressOf());
	if (FAILED(result))
	{
		e.contents = _T("shader resource view 생성 실패");
		e.errorCode = result;
		return result;
	}

	// texture의 Mipmap 생성 //
	DeviceContext->GenerateMips(m_Texture.Get());

	// 이미지 데이터 해제(명시) //
	// 불필요한 코드 but 함수가 소유권을 가지고 있다는 것을 명시적으로 나타내기 위해 작성
	ImageData.reset();

	return result;
}