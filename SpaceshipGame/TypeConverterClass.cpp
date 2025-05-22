#include "pch.h"
#include "TypeConverterClass.h"

DirectX::XMFLOAT4& TypeConverterClass::XMFLOAT3toXMFLOAT4(const DirectX::XMFLOAT3& target)
{
	DirectX::XMFLOAT4 ret;

	ret.x = target.x;
	ret.y = target.y;
	ret.z = target.z;
	ret.x = 1.f;

	return ret;
}

DirectX::XMFLOAT3& TypeConverterClass::XMFLOAT4toXMFLOAT3(const DirectX::XMFLOAT4& target)
{
	DirectX::XMFLOAT3 ret;

	ret.x = target.x;
	ret.y = target.y;
	ret.z = target.z;

	return ret;
}

DirectX::XMMATRIX& TypeConverterClass::XMFLOAT4toXMMATRIX(const DirectX::XMFLOAT4& operand1, const DirectX::XMFLOAT4& operand2, const DirectX::XMFLOAT4& operand3)
{
	using namespace DirectX;

	XMVECTOR VOperand1, VOperand2, VOperand3;
	XMMATRIX MOperand1, MOperand2, MOperand3, ret;

	// XMFLOAT4 타입을 XMVECTOR 타입으로 변환 후, XMMATRIX로 변환 //
	// 변환 과정 : XMFLOAT4 -> XMVECTOR -> XMMATRIX
	VOperand1 = XMLoadFloat4(&operand1);
	VOperand2 = XMLoadFloat4(&operand2);
	VOperand3 = XMLoadFloat4(&operand3);

	MOperand1 = XMMatrixTranslationFromVector(VOperand1);
	MOperand2 = XMMatrixRotationQuaternion(VOperand2);
	MOperand3 = XMMatrixScalingFromVector(VOperand3);

	ret = MOperand3 * MOperand2 * MOperand1;

	return ret;
}
