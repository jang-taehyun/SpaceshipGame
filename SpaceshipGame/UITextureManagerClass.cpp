#include "pch.h"
#include "TextureClass.h"
#include "UITextureManagerClass.h"

bool UI::UITextureManagerClass::IsInitialize = false;

UI::UITextureManagerClass::UITextureManagerClass() : m_CurrentUITextureMask(0)
{
	assert(!IsInitialize);
	IsInitialize = true;
}

UI::UITextureManagerClass::~UITextureManagerClass()
{
	IsInitialize = false;
}

void UI::UITextureManagerClass::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT UITextureMask)
{
	bool IsLoad = false, IsExist = false;
	Graphic::Texture::UITextureID id = Graphic::Texture::UITextureID::NONE;
	UINT flag = 0;
	std::unique_ptr<Graphic::Texture::TextureClass> texture = nullptr;

	for (UINT i = 0; i < Graphic::Texture::UITextureIDCount; ++i)
	{
		IsLoad = (UITextureMask & (1 << i));
		IsExist = (m_CurrentUITextureMask & (1 << i));
		id = static_cast<Graphic::Texture::UITextureID>(i);

		// 로드를 해야하는데 map에 없는 경우
		if (IsLoad && !IsExist)
		{
			// instance 생성
			texture = std::make_unique<Graphic::Texture::TextureClass>(Device, DeviceContext, id);
			assert(texture);

			// 현재 로드된 UI texture ID 업데이트
			m_CurrentUITextureMask |= (1 << i);

			// map에 저장
			m_TextureList.insert(std::make_pair(id, std::move(texture)));
		}
		// 해제해야 하는데 map에 있는 경우
		else if (!IsLoad && IsExist)
		{
			// instance가 실제로 존재하는지 확인하고 해제
			if (m_TextureList.end() != m_TextureList.find(id))
				m_TextureList.erase(id);

			// 현재 로드된 UI texture ID 제거
			flag = ~(1 << i);
			m_CurrentUITextureMask &= flag;
		}
	}
}
