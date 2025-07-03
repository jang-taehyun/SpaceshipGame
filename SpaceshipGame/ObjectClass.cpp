#include "pch.h"
#include "TypeConverterClass.h"
#include "ObjectClass.h"

Object::ObjectClass::ObjectClass(const AffineInfo& affine) : m_Affine(affine) {}

DirectX::XMFLOAT4X4 Object::ObjectClass::GetAffineMatrix() const
{
	DirectX::XMMATRIX affine;
	DirectX::XMFLOAT4X4 ret;

	// affine matrix = scale * rotate * pos
	affine = Utility::TypeConverterClass::XMFLOAT4toXMMATRIX(m_Affine.position, m_Affine.rotation, m_Affine.scale);

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 //
	DirectX::XMStoreFloat4x4(&ret, affine);

	return ret;
}
