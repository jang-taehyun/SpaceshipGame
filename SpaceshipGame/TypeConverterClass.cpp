#include "pch.h"
#include "TypeConverterClass.h"

DirectX::XMFLOAT4 Utility::TypeConverterClass::XMFLOAT3toXMFLOAT4(DirectX::XMFLOAT3 target)
{
	DirectX::XMFLOAT4 ret;

	ret.x = target.x;
	ret.y = target.y;
	ret.z = target.z;
	ret.x = 1.f;

	return ret;
}

DirectX::XMFLOAT3 Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(DirectX::XMFLOAT4 target)
{
	DirectX::XMFLOAT3 ret;

	ret.x = target.x;
	ret.y = target.y;
	ret.z = target.z;

	return ret;
}

DirectX::XMMATRIX Utility::TypeConverterClass::XMFLOAT4toXMMATRIX(DirectX::XMFLOAT4 operand1, DirectX::XMFLOAT4 operand2, DirectX::XMFLOAT4 operand3)
{
	using namespace DirectX;

	XMVECTOR VOperand1, VOperand2, VOperand3;
	XMMATRIX MOperand1, MOperand2, MOperand3, ret;

	// XMFLOAT4 타입을 XMVECTOR 타입으로 변환 후, XMMATRIX로 변환 //
	// 변환 과정 : XMFLOAT4 -> XMVECTOR -> XMMATRIX
	VOperand1 = XMLoadFloat4(&operand1);
	VOperand2 = XMLoadFloat4(&operand2);
	VOperand3 = XMLoadFloat4(&operand3);

	MOperand1 = (DirectX::XMVector3Equal(VOperand1, DirectX::XMVectorZero()) ? DirectX::XMMatrixIdentity() : XMMatrixTranslationFromVector(VOperand1));
	MOperand2 = (DirectX::XMVector3Equal(VOperand2, DirectX::XMVectorZero()) ? DirectX::XMMatrixIdentity() : XMMatrixRotationQuaternion(VOperand2));
	MOperand3 = (DirectX::XMVector3Equal(VOperand3, DirectX::XMVectorZero()) ? DirectX::XMMatrixIdentity() : XMMatrixScalingFromVector(VOperand3));

	ret = MOperand3 * MOperand2 * MOperand1;

	return ret;
}
