#include "pch.h"
#include "TextureClass.h"
#include "UITextureManagerClass.h"

bool UI::UITextureManagerClass::IsInitialize = false;

UI::UITextureManagerClass::UITextureManagerClass()
{
	IsInitialize = true;
}

UI::UITextureManagerClass::~UITextureManagerClass()
{
	ReleaseUITexture();
	IsInitialize = false;
}

void UI::UITextureManagerClass::LoadUITexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT TextureIDs)
{
	UINT flag = 1;
	bool IsLoad = false;
	std::unique_ptr<Graphic::Texture::TextureClass> texture;
	std::map<Graphic::Texture::UITextureID, std::unique_ptr<Graphic::Texture::TextureClass>>::iterator iter;

	// UI texture 로드 //
	while (flag)
	{
		IsLoad = (TextureIDs & flag);

		// 해당 UI texture 로드 //
		if (IsLoad)
		{
			// 현재 UI texture가 이미 로드되어 있는지 확인
			iter = m_TextureList.find(static_cast<Graphic::Texture::UITextureID>(flag));

			// 로드가 되어 있지 않다면 로드
			if (m_TextureList.end() == iter)
			{
				texture = std::make_unique<Graphic::Texture::TextureClass>(Device, DeviceContext, Graphic::Texture::UITextureList.at(static_cast<Graphic::Texture::UITextureID>(flag)));
				m_TextureList.insert(std::make_pair(static_cast<Graphic::Texture::UITextureID>(flag), std::move(texture)));
			}
		}
		// 해당 UI texture 로드 안함(삭제) //
		else
			m_TextureList.erase(static_cast<Graphic::Texture::UITextureID>(flag));

		flag = flag << 1;
	}
}

void UI::UITextureManagerClass::ReleaseUITexture()
{
	m_TextureList.clear();
}
