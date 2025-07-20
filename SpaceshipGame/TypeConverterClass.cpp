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

DirectX::XMFLOAT4X4 Utility::TypeConverterClass::XMFLOAT4toXMFLOAT4X4(DirectX::XMFLOAT4 operand1, DirectX::XMFLOAT4 operand2, DirectX::XMFLOAT4 operand3)
{
	using namespace DirectX;

	XMVECTOR VOperand1, VOperand2, VOperand3;
	XMMATRIX MOperand1, MOperand2, MOperand3, tmp;
	XMFLOAT4X4 ret = XMFLOAT4X4();

	// XMFLOAT4 타입을 XMVECTOR, XMMATRIX 타입으로 변환 후, XMFLOAT4X4로 변환 //
	// 변환 과정 : XMFLOAT4 -> XMVECTOR -> XMMATRIX -> XMFLOAT4X4
	VOperand1 = XMLoadFloat4(&operand1);
	VOperand2 = XMQuaternionRotationRollPitchYaw(operand2.x, operand2.y, operand2.z);
	VOperand3 = XMLoadFloat4(&operand3);

	MOperand1 = (DirectX::XMVector3Equal(VOperand1, DirectX::XMVectorZero()) ? DirectX::XMMatrixIdentity() : XMMatrixTranslationFromVector(VOperand1));
	MOperand2 = (DirectX::XMVector3Equal(VOperand2, DirectX::XMVectorZero()) ? DirectX::XMMatrixIdentity() : XMMatrixRotationQuaternion(VOperand2));
	MOperand3 = (DirectX::XMVector3Equal(VOperand3, DirectX::XMVectorZero()) ? DirectX::XMMatrixIdentity() : XMMatrixScalingFromVector(VOperand3));
	tmp = MOperand3 * MOperand2 * MOperand1;

	// XMMATRIX 타입을 XMFLOAT4X4 타입으로 변환 //
	DirectX::XMStoreFloat4x4(&ret, tmp);
	return ret;
}
