#pragma once

class LightClass
{
public:
	explicit LightClass(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT4& speculrColor, const float& specularPower);
	virtual ~LightClass();

	// Getter //

	inline const DirectX::XMFLOAT4& GetAmbientColor() const { return m_AmbientColor; }
	inline const DirectX::XMFLOAT4& GetDiffuseColor() const { return m_DiffuseColor; }
	inline const DirectX::XMFLOAT3& GetDirection() const { return m_LightDirection; }
	inline const DirectX::XMFLOAT4& GetSpecularColor() const { return m_SpecularColor; }
	inline const float& GetSpecularPower() const { return m_SpecularPower; }

	// Setter //

	inline void SetAmbientColor(const float& r, const float& g, const float& b, const float& a) { m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetAmbientColor(const DirectX::XMFLOAT4& ambient) { m_AmbientColor = ambient; }
	inline void SetDiffuseColor(const float& r, const float& g, const float& b, const float& a) { m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetDiffuseColor(const DirectX::XMFLOAT4& diffuse) { m_DiffuseColor = diffuse; }
	inline void SetDirection(const float& x, const float& y, const float& z) { m_LightDirection = DirectX::XMFLOAT3(x, y, z); }
	inline void SetDirection(const DirectX::XMFLOAT3& direction) { m_LightDirection = direction; }
	inline void SetSpecularColor(const float& r, const float& g, const float& b, const float& a) { m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetSpecularColor(const DirectX::XMFLOAT4& specular_color) { m_SpecularColor = specular_color; }
	inline void SetSpecularPower(const float& power) { m_SpecularPower = power; }

	// operator overloading //
	LightClass& operator=(const LightClass& other);

private:
	DirectX::XMFLOAT4 m_AmbientColor;
	DirectX::XMFLOAT4 m_DiffuseColor;
	DirectX::XMFLOAT3 m_LightDirection;
	DirectX::XMFLOAT4 m_SpecularColor;
	float m_SpecularPower;

public:
	LightClass() = delete;
	LightClass(const LightClass& other) = delete;
};

