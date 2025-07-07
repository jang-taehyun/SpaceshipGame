#include "pch.h"
#include "TextureClass.h"
#include "UITextureManagerClass.h"

bool Graphic::Texture::UITextureManagerClass::IsInitialize = false;

Graphic::Texture::UITextureManagerClass::UITextureManagerClass() : m_CurrentUITextureMask(0)
{
	assert(!IsInitialize);
	IsInitialize = true;
}

Graphic::Texture::UITextureManagerClass::~UITextureManagerClass()
{
	IsInitialize = false;
}

ID3D11ShaderResourceView* Graphic::Texture::UITextureManagerClass::GetTexture(UITextureID id) const
{
	std::map<UITextureID, std::unique_ptr<TextureClass>>::const_iterator iter;

	iter = m_TextureList.find(id);
	if (m_TextureList.end() == iter)
		return nullptr;

	return iter->second->GetTexture();
}

void Graphic::Texture::UITextureManagerClass::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT UITextureMask)
{
	bool IsLoad = false, IsExist = false;
	UITextureID id = UITextureID::NONE;
	UINT flag = 0;
	std::unique_ptr<TextureClass> texture = nullptr;

	for (UINT i = 0; i < UITextureIDCount; ++i)
	{
		IsLoad = (UITextureMask & (1 << i));
		IsExist = (m_CurrentUITextureMask & (1 << i));
		id = static_cast<UITextureID>(i);

		// 로드를 해야하는데 map에 없는 경우
		if (IsLoad && !IsExist)
		{
			// instance 생성
			texture = std::make_unique<TextureClass>(Device, DeviceContext, id);
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
