#pragma once

class LightClass
{
public:
	LightClass(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 direction, DirectX::XMFLOAT4 speculrColor, float specularPower);
	virtual ~LightClass() = default;

	// Getter //

	inline DirectX::XMFLOAT4 GetAmbientColor() const { return m_AmbientColor; }
	inline DirectX::XMFLOAT4 GetDiffuseColor() const { return m_DiffuseColor; }
	inline DirectX::XMFLOAT4 GetDirection() const { return m_LightDirection; }
	inline DirectX::XMFLOAT4 GetSpecularColor() const { return m_SpecularColor; }
	inline float GetSpecularPower() const { return m_SpecularPower; }

	// Setter //

	inline void SetAmbientColor(float r, float g, float b, float a) { m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetAmbientColor(DirectX::XMFLOAT4 ambient) { m_AmbientColor = ambient; }
	inline void SetDiffuseColor(float r, float g, float b, float a) { m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetDiffuseColor(DirectX::XMFLOAT4 diffuse) { m_DiffuseColor = diffuse; }
	inline void SetDirection(float x, float y, float z) { m_LightDirection = DirectX::XMFLOAT4(x, y, z, 1.f); }
	inline void SetDirection(DirectX::XMFLOAT4 direction) { m_LightDirection = direction; }
	inline void SetSpecularColor(float r, float g, float b, float a) { m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetSpecularColor(DirectX::XMFLOAT4 specular_color) { m_SpecularColor = specular_color; }
	inline void SetSpecularPower(float power) { m_SpecularPower = power; }

private:
	DirectX::XMFLOAT4 m_AmbientColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_DiffuseColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_LightDirection = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_SpecularColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	float m_SpecularPower = 0.f;
};

