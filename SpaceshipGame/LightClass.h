#pragma once

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	DirectX::XMFLOAT4 GetAmbientColor();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT4 GetSpecularColor();
	float GetSpecularPower();

private:
	DirectX::XMFLOAT4 m_AmbientColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_DiffuseColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 m_Direction = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 m_SpecularColor = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	float m_SpecularPower = 0.f;
};

