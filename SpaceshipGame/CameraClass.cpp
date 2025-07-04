#include "pch.h"
#include "ComputeDirectionVectorClass.h"
#include "CameraClass.h"

DirectX::XMFLOAT4X4 Object::CameraClass::Render() const
{
	DirectX::XMFLOAT4 pos, rot;
	DirectX::XMVECTOR position, forward, target, up;
	DirectX::XMFLOAT4 ForwardVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 UpVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 RightVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMFLOAT4X4 ret;

	// 카메라의 local 좌표계의 축 추출 //
	rot = GetRotation();
	ComputeDirectionVectorClass::GetDirectionVectors(rot, ForwardVector, RightVector, UpVector);
	forward = DirectX::XMLoadFloat4(&ForwardVector);
	up = DirectX::XMLoadFloat4(&UpVector);

	// 카메라의 world 좌표계의 position 추출 //
	pos = GetPosition();
	position = DirectX::XMLoadFloat4(&pos);

	// target vector 계산 //
	target = DirectX::XMVectorAdd(position, forward);

	// view matrix 생성 //
	viewMatrix = DirectX::XMMatrixLookAtLH(position, target, up);
	DirectX::XMStoreFloat4x4(&ret, viewMatrix);

	return ret;
}
