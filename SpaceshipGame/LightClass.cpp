#include "pch.h"
#include "LightClass.h"

Graphic::LightClass::LightClass(DirectX::XMFLOAT4 ambient, DirectX::XMFLOAT4 diffuse, DirectX::XMFLOAT4 direction, DirectX::XMFLOAT4 speculrColor, float specularPower)
	: m_AmbientColor(ambient),
	m_DiffuseColor(diffuse),
	m_LightDirection(direction),
	m_SpecularColor(speculrColor),
	m_SpecularPower(specularPower)
{}