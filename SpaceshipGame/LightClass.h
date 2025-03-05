#pragma once

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	// Getter //

	inline const DirectX::XMFLOAT4& GetAmbientColor() const { return m_AmbientColor; }
	inline const DirectX::XMFLOAT4& GetDiffuseColor() const { return m_DiffuseColor; }
	inline const DirectX::XMFLOAT3& GetDirection() const { return m_Direction; }
	inline const DirectX::XMFLOAT4& GetSpecularColor() const { return m_SpecularColor; }
	inline const float& GetSpecularPower() const { return m_SpecularPower; }

	// Setter //

	inline void SetAmbientColor(const float& r, const float& g, const float& b, const float& a) { m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetDiffuseColor(const float& r, const float& g, const float& b, const float& a) { m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetDirection(const float& x, const float& y, const float& z) { m_Direction = DirectX::XMFLOAT3(x, y, z); }
	inline void SetSpecularColor(const float& r, const float& g, const float& b, const float& a) { m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetSpecularPower(const float& power) { m_SpecularPower = power; }

private:
	DirectX::XMFLOAT4 m_AmbientColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_DiffuseColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 m_Direction = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_SpecularColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	float m_SpecularPower = 0.f;
};

