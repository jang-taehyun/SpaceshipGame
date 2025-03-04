#pragma once

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass& other);
	~LightClass();

	// Getter //

	inline const DirectX::XMFLOAT4& const GetAmbientColor() { return m_AmbientColor; }
	inline const DirectX::XMFLOAT4& const GetDiffuseColor() { return m_DiffuseColor; }
	inline const DirectX::XMFLOAT3& const GetDirection() { return m_Direction; }
	inline const DirectX::XMFLOAT4& const GetSpecularColor() { return m_SpecularColor; }
	inline const float& const GetSpecularPower() { return m_SpecularPower; }

	// Setter //

	inline void SetAmbientColor(const float& const r,  const float& const g, const float& const b, const float& const a) { m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetDiffuseColor(const float& const r, const float& const g, const float& const b, const float& const a) { m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetDirection(const float& const x, const float& const y, const float& const z){ m_Direction = DirectX::XMFLOAT3(x, y, z); }
	inline void SetSpecularColor(const float& const r, const float& const g, const float& const b, const float& const a) { m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a); }
	inline void SetSpecularPower(const float& const power) { m_SpecularPower = power; }

private:
	DirectX::XMFLOAT4 m_AmbientColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_DiffuseColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 m_Direction = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_SpecularColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	float m_SpecularPower = 0.f;
};

