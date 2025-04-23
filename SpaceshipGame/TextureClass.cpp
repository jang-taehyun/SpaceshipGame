#include "pch.h"
#include <fstream>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include "TextureClass.h"

static ErrorContent e;

TextureClass::TextureClass(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& FileNames)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass Constructor");

	result = Initialize(Device, DeviceContext, FileNames);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

TextureClass::~TextureClass()
{
	Shutdown();
}

HRESULT TextureClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& FileNames)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass Initialize()");

	std::wstring Extension;
	size_t DotIdx;

	for (int i = 0; i < FileNames.size(); ++i)
	{
		// 매개변수 검사 //
		if (_T("") == FileNames[i])
		{
			e.contents = _T("필요한 파일 정보(경로, 이름)이 없습니다.");
			e.errorCode = E_FAIL;
			return E_FAIL;
		}

		// 파일의 확장자 추출
		if (FileNames[i].rfind('.') == std::string::npos)
		{
			e.contents = _T("필요한 파일 정보(경로, 이름)에 확장자가 없습니다.");
			e.errorCode = E_FAIL;
			return E_FAIL;
		}

		DotIdx = FileNames[i].rfind('.');
		Extension = FileNames[i].substr(DotIdx + (size_t)1);

		// 파일의 확장자에 따라 이미지 로드 함수를 호출해 메모리에 이미지 데이터 로드
		if (_T("png") == Extension || _T("PNG") == Extension || _T("jpg") == Extension || _T("JPG") == Extension)
		{
			result = LoadPNG(Device, FileNames[i]);
			if (FAILED(result))
				return result;
		}
		else if (_T("tga") == Extension || _T("TGA") == Extension)
		{
			result = LoadTarga(FileNames[i]);
			if (FAILED(result))
				return result;

			// 이미지 데이터에 맞는 SRV 생성 //
			result = CreateShaderResourceView(Device, DeviceContext);
			if (FAILED(result))
				return result;
		}
		else if (_T("dds") == Extension || _T("DDS") == Extension)
		{
			result = LoadDDS(Device, FileNames[i]);
			if (FAILED(result))
				return result;
		}
	}

	return result;
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

	if (m_ImageData)
	{
		delete[] m_ImageData;
		m_ImageData = nullptr;
	}
}

HRESULT TextureClass::LoadTarga(const std::wstring& FileName)
{
	HRESULT result = S_OK;
	std::ifstream FileIn;
	TargaHeader TargaFileHeader;			// targa 파일의 header 정보
	int bpp = 0;							// targa 파일의 색상 bit 수(32bit 또는 24bit)
	int ImageSize = 0;						// targa 이미지의 크기
	char* TargaImage = nullptr;				// targa 이미지 데이터
	UINT count = 0;
	int index = 0, k = 0;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadTarga()");

	// targa 파일을 binary 모드로 열기 //
	FileIn.open(FileName, std::ios::in | std::ios::binary);
	if (FileIn.fail())
	{
		return E_FAIL;
	}

	// targa 파일의 header를 가져오기 //
	FileIn.read((char*)&TargaFileHeader, sizeof(TargaFileHeader));
	count = static_cast<unsigned int>(FileIn.gcount());
	if (sizeof(TargaFileHeader) != count)
	{
		FileIn.close();
		e.contents = _T("targa 파일의 header를 가져오기 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// targa 파일의 header에서 중요 정보 가져오기 //
	m_Height = TargaFileHeader.height;
	m_Width = TargaFileHeader.width;
	bpp = TargaFileHeader.bpp;

	// targa 파일이 32 bit인지 24 bit인지 확인 //
	if (bpp != 32)
	{
		FileIn.close();
		e.contents = _T("targa 파일이 32bit가 아닙니다.");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// 32 bit 이미지 데이터의 크기 계산 //
	ImageSize = m_Width * m_Height * 4;

	// targa 이미지 데이터용 메모리 할당 //
	TargaImage = new char[ImageSize];
	if (!TargaImage)
	{
		FileIn.close();
		e.contents = _T("targa 이미지 데이터용 메모리 할당 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// targa 이미지 데이터 읽기 //
	FileIn.read(TargaImage, ImageSize);
	count = static_cast<unsigned int>(FileIn.gcount());
	if (count != ImageSize)
	{
		if (TargaImage)
		{
			delete[] TargaImage;
			TargaImage = nullptr;
		}

		FileIn.close();
		e.contents = _T("targa 이미지 데이터 읽기 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// targa 이미지 파일 닫기 //
	FileIn.close();

	// targa 대상 데이터에 대한 메모리 할당 //
	m_ImageData = new unsigned char[ImageSize];
	if (!m_ImageData)
	{
		if (TargaImage)
		{
			delete[] TargaImage;
			TargaImage = nullptr;
		}

		e.contents = _T("targa 대상 데이터에 대한 메모리 할당 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// targa 대상 데이터 배열에 index 초기화 //
	index = 0;

	// targa 이미지 데이터에 index 초기화 //
	// targa 이미지 데이터의 읽기 시작 위치
	// targa 데이터의 마지막 행에서 데이터의 읽기 시작
	k = (m_Width * m_Height * 4) - (m_Width * 4);

	// targa 데이터를 정렬하여 메모리에 저장 //
	// targa 형식이 거꾸로 저장되었으므로 올바른 순서로 targa 이미지 데이터를 targa 대상 배열에 복사
	// 복사될 위치는 위에서 아래로, targa 이미지 데이터의 위치(복사할 위치)는 아래에서 위로 진행
	for (int j = 0; j < m_Height; j++)
	{
		// targa 데이터의 가로(width) 데이터를 메모리에 복사
		for (int i = 0; i < m_Width; i++)
		{
			if ((index + 3) < ImageSize && (k + 3) < ImageSize)
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
		}

		// targa 이미지 데이터 인덱스를 역순으로 읽은 후 열의 시작 부분에서 이전 행으로 다시 설정
		k -= (m_Width * 8);
	}

	// targa 이미지 데이터 해제 //
	delete[] TargaImage;
	TargaImage = nullptr;

	return result;
}

HRESULT TextureClass::LoadPNG(ID3D11Device* const& Device, const std::wstring& FileName)
{
	HRESULT result = S_OK;
	ID3D11ShaderResourceView* srv = nullptr;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadPNG()");

	result = DirectX::CreateWICTextureFromFile(Device, FileName.c_str(), nullptr, &srv);
	if (FAILED(result))
	{
		e.contents = _T("WIC texture 파일(png, jpg 등) load 실패");
		e.errorCode = result;
		return result;
	}

	m_TextureView.push_back(srv);

	return result;
}

HRESULT TextureClass::LoadDDS(ID3D11Device* const& Device, const std::wstring& FileName)
{
	HRESULT result = S_OK;
	ID3D11ShaderResourceView* srv = nullptr;

	// 에러 메세지 초기화 //
	e.title = _T("TextureClass LoadDDS()");

	result = DirectX::CreateDDSTextureFromFile(Device, FileName.c_str(), nullptr, &srv);
	if (FAILED(result))
	{
		e.contents = _T("dds texture 파일 load 실패");
		e.errorCode = result;
		return result;
	}

	m_TextureView.push_back(srv);

	return result;
}

HRESULT TextureClass::CreateShaderResourceView(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext)
{
	HRESULT result = S_OK;
	D3D11_TEXTURE2D_DESC TextureDesc;							// texture 설정 정보
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;		// shader resoure view 설정 정보
	ID3D11Texture2D* texture = nullptr;							// texture
	ID3D11ShaderResourceView* srv = nullptr;					// shader resoure view
	UINT RowPitch = 0;

	// 에러 메세지, 구조체 초기화 //
	e.title = _T("TextureClass LoadDDS()");
	memset(&TextureDesc, 0, sizeof(TextureDesc));
	memset(&ShaderResourceViewDesc, 0, sizeof(ShaderResourceViewDesc));
	
	// 빈 texture 생성 //
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
	result = Device->CreateTexture2D(&TextureDesc, NULL, &texture);
	if (FAILED(result))
	{
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}
		if (srv)
		{
			srv->Release();
			srv = nullptr;
		}

		e.contents = _T("빈 texture 생성 실패");
		e.errorCode = result;
		return result;
	}

	// 이미지 데이터를 빈 texture에 복사 //

	// 이미지 데이터의 width의 크기(바이트 크기) 구하기
	// 이미지는 RGBA 형식이므로, 한 pixel의 크기는 4byte
	RowPitch = (m_Width * 4) * sizeof(unsigned char);

	// 이미지 데이터를 texture에 복사
	DeviceContext->UpdateSubresource(texture, 0, NULL, m_ImageData, RowPitch, 0);


	// shader resource view 생성 //
	// shader resource view 설정
	ShaderResourceViewDesc.Format = TextureDesc.Format;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	ShaderResourceViewDesc.Texture2D.MipLevels = -1;

	// shader resource view 생성
	result = Device->CreateShaderResourceView(texture, &ShaderResourceViewDesc, &srv);
	if (FAILED(result))
	{
		if (texture)
		{
			texture->Release();
			texture = nullptr;
		}
		if (srv)
		{
			srv->Release();
			srv = nullptr;
		}

		e.contents = _T("shader resource view 생성 실패");
		e.errorCode = result;
		return result;
	}

	// texture의 Mipmap 생성 //
	DeviceContext->GenerateMips(srv);

	// 생성된 texture resource view를 멤버 변수에 저장 //
	m_TextureView.push_back(srv);

	// 이미지 데이터 해제 //
	delete[] m_ImageData;
	m_ImageData = nullptr;

	return result;
}