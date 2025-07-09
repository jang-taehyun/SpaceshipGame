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

bool Object::CameraClass::IsRender(DirectX::BoundingOrientedBox ModelOBB, DirectX::XMFLOAT4X4 ModelWorldMatrix) const
{
	DirectX::BoundingOrientedBox comp = ModelOBB;
	bool IsContain = false, IsIntersect = false;

	// World matrix를 XMMATRIX로 변환 //
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&ModelWorldMatrix);

	// model의 OBB 박스를 World matrix를 이용해 trasform 변환 //
	comp.Transform(comp, world);

	// frustum volume 안에 있는지 체크 //
	// 교차하는지 체크
	IsIntersect = m_Frustum.Intersects(comp);
	// 포함하는지 체크
	IsContain = m_Frustum.Contains(comp);

	return (IsIntersect || IsContain);
}

void Object::CameraClass::UpdateFrustum(DirectX::XMFLOAT4X4 projection)
{
	DirectX::XMFLOAT4X4 worldF = GetAffineMatrix();

	// projection matrix를 XMMATRIX로 변환
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&projection);

	// World matrix를 XMMATRIX로 변환
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&worldF);

	// projection matrix를 통해 frustum volume을 생성하고, world matrix를 이용해 trasform 변환
	m_Frustum = DirectX::BoundingFrustum(proj);
	m_Frustum.Transform(m_Frustum, world);
}
