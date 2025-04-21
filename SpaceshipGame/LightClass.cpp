#include "pch.h"
#include "LightClass.h"

LightClass::LightClass(const DirectX::XMFLOAT4& ambient, const DirectX::XMFLOAT4& diffuse, const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT4& speculrColor, const float& specularPower)
{
	m_AmbientColor = ambient;
	m_DiffuseColor = diffuse;
	m_LightDirection = direction;
	m_SpecularColor = speculrColor;
	m_SpecularPower = specularPower;
}

LightClass::~LightClass() {}

LightClass& LightClass::operator=(const LightClass& other)
{
	this->m_AmbientColor = other.m_AmbientColor;
	this->m_DiffuseColor = other.m_DiffuseColor;
	this->m_LightDirection = other.m_LightDirection;
	this->m_SpecularPower = other.m_SpecularPower;
	this->m_SpecularColor = other.m_SpecularColor;

	return (*this);
}

