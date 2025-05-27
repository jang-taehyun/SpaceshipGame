#include "pch.h"
#include "TypeConverterClass.h"
#include "GameObjectClass.h"

const DirectX::XMFLOAT4X4& GameObjectClass::GetAffineMatrix() const
{
	DirectX::XMMATRIX affine;
	DirectX::XMFLOAT4X4 ret;

	// affine matrix = scale * rotate * pos
	affine = TypeConverterClass::XMFLOAT4toXMMATRIX(m_Position, m_Rotation, m_Scale);

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 후 반환 //
	DirectX::XMStoreFloat4x4(&ret, affine);
	return ret;
}