#pragma once

/**
* ComputeDirectionVectorClass °³¿ä
*/

namespace Object
{
	class ComputeDirectionVectorClass
	{
	public:
		static void GetDirectionVectors(DirectX::XMFLOAT4 rotate, DirectX::XMFLOAT4 forward, DirectX::XMFLOAT4 right, DirectX::XMFLOAT4 up);
		static DirectX::XMFLOAT4 GetForwardVector(DirectX::XMFLOAT4 rotate);
		static DirectX::XMFLOAT4 GetRightVector(DirectX::XMFLOAT4 rotate);
		static DirectX::XMFLOAT4 GetUpVector(DirectX::XMFLOAT4 rotate);
	};
}