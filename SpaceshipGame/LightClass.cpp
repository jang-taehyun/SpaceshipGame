#include "pch.h"
#include "LightClass.h"

LightClass::LightClass() {}
LightClass::LightClass(const LightClass& other) {}
LightClass::~LightClass() {}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_AmbientColor = DirectX::XMFLOAT4(r, g, b, a);
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_DiffuseColor = DirectX::XMFLOAT4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_Direction = DirectX::XMFLOAT3(x, y, z);
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
	m_SpecularColor = DirectX::XMFLOAT4(r, g, b, a);
}

void LightClass::SetSpecularPower(float power)
{
	m_SpecularPower = power;
}

DirectX::XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_AmbientColor;
}

DirectX::XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_DiffuseColor;
}

DirectX::XMFLOAT3 LightClass::GetDirection()
{
	return m_Direction;
}

DirectX::XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_SpecularColor;
}

float LightClass::GetSpecularPower()
{
	return m_SpecularPower;
}
