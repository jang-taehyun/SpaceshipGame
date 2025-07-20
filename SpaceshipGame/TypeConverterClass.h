#pragma once

/**
* TypeConverterClass 개요
* - 여러 자료형 간의 변환과 관련된 함수 모음
*/
namespace Utility
{
	class TypeConverterClass
	{
	public:
		// XMFLOAT3 -> XMFLOAT4
		static DirectX::XMFLOAT4 XMFLOAT3toXMFLOAT4(DirectX::XMFLOAT3 target);

		// XMFLOAT4 -> XMFLOAT3
		static DirectX::XMFLOAT3 XMFLOAT4toXMFLOAT3(DirectX::XMFLOAT4 target);

		// XMFLOAT4 -> XMFLOAT4X4
		static DirectX::XMFLOAT4X4 XMFLOAT4toXMFLOAT4X4(DirectX::XMFLOAT4 operand1, DirectX::XMFLOAT4 operand2, DirectX::XMFLOAT4 operand3);
	};
}