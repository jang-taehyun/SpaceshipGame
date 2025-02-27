#include "pch.h"
#include "DDSTextureLoader.h"
#include "TextureArrayClass.h"

TextureArrayClass::TextureArrayClass() {}
TextureArrayClass::TextureArrayClass(const TextureArrayClass& other) {}
TextureArrayClass::~TextureArrayClass() {}

bool TextureArrayClass::Initialize(ID3D11Device* Device, const TCHAR* DefaultTextureFilename, const TCHAR* ColorTextureFilename)
{
    // default texture file를 메모리에 load //
	if (FAILED(DirectX::CreateDDSTextureFromFile(Device, DefaultTextureFilename, nullptr, &m_Textures[0])))
    {
        return false;
    }

    // color texture file를 메모리에 load //
    if (FAILED(DirectX::CreateDDSTextureFromFile(Device, ColorTextureFilename, nullptr, &m_Textures[1])))
    {
        return false;
    }

	return true;
}

bool TextureArrayClass::Initialize(ID3D11Device* Device, const TCHAR* DefaultTextureFilename, const TCHAR* ColorTextureFilename, const TCHAR* AlphaMapTextureFilename)
{
    // default texture file를 메모리에 load //
    if (FAILED(DirectX::CreateDDSTextureFromFile(Device, DefaultTextureFilename, nullptr, &m_Textures[0])))
    {
        return false;
    }

    // color texture file를 메모리에 load //
    if (FAILED(DirectX::CreateDDSTextureFromFile(Device, ColorTextureFilename, nullptr, &m_Textures[1])))
    {
        return false;
    }

    // alpha map texture file를 메모리에 load //
    if (FAILED(DirectX::CreateDDSTextureFromFile(Device, AlphaMapTextureFilename, nullptr, &m_Textures[2])))
    {
        return false;
    }

    return true;
}

void TextureArrayClass::Shutdown()
{
    if (m_Textures[2])
    {
        m_Textures[2]->Release();
        m_Textures[2] = nullptr;
    }

    if (m_Textures[1])
    {
        m_Textures[1]->Release();
        m_Textures[1] = nullptr;
    }

    if (m_Textures[0])
    {
        m_Textures[0]->Release();
        m_Textures[0] = nullptr;
    }
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
    return m_Textures;
}
