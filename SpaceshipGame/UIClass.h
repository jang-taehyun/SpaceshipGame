#pragma once

#include <SpriteBatch.h>

class TextureClass;

class UIClass
{
public:
	UIClass(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext, const std::wstring& Filename, DirectX::XMFLOAT2 Position = DirectX::XMFLOAT2(0.f, 0.f), float Rotation = 0.f, DirectX::XMFLOAT2 Origin = DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT2 Scale = DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	virtual ~UIClass() = default;

	virtual void Render() const;

	inline DirectX::XMFLOAT2 GetPosition() const { return m_Position; }
	inline float GetRotation() const { return m_Rotation; }
	inline DirectX::XMFLOAT2 GetOrigin() const { return m_Origin; }
	inline DirectX::XMFLOAT2 GetScale() const { return m_Scale; }
	inline DirectX::XMFLOAT4 GetColor() const { return m_Color; }

	inline void SetPosition(DirectX::XMFLOAT2 pos) { m_Position = pos; }
	inline void SetRotation(float rot) { m_Rotation = rot; }
	inline void SetOrigin(DirectX::XMFLOAT2 origin) { m_Origin = origin; }
	inline void SetScale(DirectX::XMFLOAT2 scale) { m_Scale = scale; }
	inline void SetColor(DirectX::XMFLOAT4 color) { m_Color = color; }

private:
	std::unique_ptr<DirectX::SpriteBatch> m_Render = nullptr;
	std::unique_ptr<TextureClass> m_Texture = nullptr;

	DirectX::XMFLOAT2 m_Position = DirectX::XMFLOAT2(0.f, 0.f);
	DirectX::XMFLOAT2 m_Origin = DirectX::XMFLOAT2(0.f, 0.f);
	float m_Rotation = 0.f;
	DirectX::XMFLOAT2 m_Scale = DirectX::XMFLOAT2(0.f, 0.f);

	DirectX::XMFLOAT4 m_Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
};

