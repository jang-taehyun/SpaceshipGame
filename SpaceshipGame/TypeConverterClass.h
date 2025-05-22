#pragma once

/**
* TypeConverterClass 개요
* - 여러 자료형 간의 변환과 관련된 함수 모음
*/

class TypeConverterClass
{
public:
	TypeConverterClass() = default;
	virtual ~TypeConverterClass() = default;

	// XMFLOAT3 -> XMFLOAT4
	static DirectX::XMFLOAT4& XMFLOAT3toXMFLOAT4(const DirectX::XMFLOAT3& target);

	// XMFLOAT4 -> XMFLOAT3
	static DirectX::XMFLOAT3& XMFLOAT4toXMFLOAT3(const DirectX::XMFLOAT4& target);

	// XMFLOAT4 -> XMMATRIX
	static DirectX::XMMATRIX& XMFLOAT4toXMMATRIX(const DirectX::XMFLOAT4& operand1, const DirectX::XMFLOAT4& operand2, const DirectX::XMFLOAT4& operand3);
};