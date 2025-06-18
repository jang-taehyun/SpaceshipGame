#include "pch.h"
#include "TextureClass.h"
#include "UIClass.h"

UIClass::UIClass(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext, const std::wstring& Filename, DirectX::XMFLOAT2 Position, float Rotation, DirectX::XMFLOAT2 Origin, DirectX::XMFLOAT2 Scale, DirectX::XMFLOAT4 Color) : m_Position(Position), m_Rotation(Rotation), m_Origin(Origin), m_Scale(Scale), m_Color(Color)
{
	m_Render = std::make_unique<DirectX::SpriteBatch>(DeviceContext);
	m_Texture = std::make_unique<TextureClass>(Device, DeviceContext, Filename);
}

void UIClass::Render() const
{
	DirectX::XMVECTOR color;
	color = DirectX::XMLoadFloat4(&m_Color);

	m_Render->Begin();
	m_Render->Draw(const_cast<ID3D11ShaderResourceView*>(m_Texture->GetTexture()), m_Position, nullptr, color, m_Rotation, m_Origin);
	m_Render->End();
}