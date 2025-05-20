#include "pch.h"
#include "AffineClass.h"

AffineClass::AffineClass(const AffineInfo& affine)
{
	Initialize(affine);
}

void AffineClass::Initialize(const AffineInfo& affine)
{
	m_Position = affine.position;
	m_Rotation = affine.rotation;
	m_Scaling = affine.scale;
}
