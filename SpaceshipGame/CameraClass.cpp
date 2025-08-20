#include "pch.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "ComputeDirectionVectorClass.h"
#include "CameraClass.h"

Object::CameraClass::CameraClass(std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate) : m_Move(std::move(move)), m_Rotate(std::move(rotate))
{}

Object::CameraClass::CameraClass(const CameraClass& other) : ObjectClass(other), m_Move(other.m_Move->Clone()), m_Rotate(other.m_Rotate->Clone()), m_Frustum(other.m_Frustum)
{}

Object::CameraClass::CameraClass(CameraClass && other) noexcept : ObjectClass(other), m_Move(std::move(other.m_Move)), m_Rotate(std::move(other.m_Rotate)), m_Frustum(other.m_Frustum)
{}

DirectX::XMFLOAT4X4 Object::CameraClass::Render()
{
	DirectX::XMFLOAT4 pos, rot;
	DirectX::XMVECTOR position, forward, target, up;
	DirectX::XMFLOAT4 ForwardVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 UpVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMFLOAT4 RightVector = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 0.f);
	DirectX::XMMATRIX viewMatrix;

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
	DirectX::XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	return m_ViewMatrix;
}

bool Object::CameraClass::IsRenderModel(DirectX::BoundingOrientedBox ModelOBB, DirectX::XMFLOAT4X4 ModelWorldMatrix) const
{
	DirectX::BoundingOrientedBox comp(ModelOBB);
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

bool Object::CameraClass::IsRenderQuadTree(DirectX::XMFLOAT4 point1, DirectX::XMFLOAT4 point2)
{
	DirectX::BoundingBox comp;
	bool IsContain = false, IsIntersect = false;
	
	// Quad Tree에 대한 AABB 박스 생성 //
	DirectX::XMVECTOR p1 = DirectX::XMLoadFloat4(&point1);
	DirectX::XMVECTOR p2 = DirectX::XMLoadFloat4(&point2);
	DirectX::BoundingBox::CreateFromPoints(comp, p1, p2);

	// frustum volume 안에 있는지 체크 //
	// 교차하는지 체크
	IsIntersect = m_Frustum.Intersects(comp);
	// 포함하는지 체크
	IsContain = m_Frustum.Contains(comp);

	return (IsIntersect || IsContain);

	return false;
}

Object::CameraClass& Object::CameraClass::operator=(const CameraClass& other)
{
	if (this == &other)
		return *this;

	m_Frustum = other.m_Frustum;

	if (m_Move)
		m_Move.reset();
	if (m_Rotate)
		m_Rotate.reset();

	m_Move = other.m_Move->Clone();
	m_Rotate = other.m_Rotate->Clone();

	ObjectClass::operator=(other);

	return *this;
}

Object::CameraClass& Object::CameraClass::operator=(CameraClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_Frustum = other.m_Frustum;

	if (m_Move)
		m_Move.reset();
	if (m_Rotate)
		m_Rotate.reset();

	m_Move = std::move(other.m_Move);
	m_Rotate = std::move(other.m_Rotate);

	ObjectClass::operator=(std::move(other));

	return *this;
}

void Object::CameraClass::UpdateFrustum(DirectX::XMFLOAT4X4 projection)
{
	// projection matrix를 XMMATRIX로 변환
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&projection);

	// view matrix를 XMMATRIX로 변환
	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&m_ViewMatrix);

	// projection matrix를 통해 frustum volume을 생성하고, world matrix를 이용해 trasform 변환
	m_Frustum = DirectX::BoundingFrustum(proj);
	view = DirectX::XMMatrixInverse(nullptr, view);
	m_Frustum.Transform(m_Frustum, view);
}

void Object::CameraClass::Move(MoveState state, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 pos;

	// 최종적으로 계산된 position으로 교체 //
	// actor 이동
	if (!m_Move)
		return;
	pos = m_Move->Move(GetPosition(), GetRotation(), state, frame_time, IsKeyDown);
	SetPosition(pos);
}

void Object::CameraClass::Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 rot;

	// 최종적으로 계산된 rotation으로 교체 //
	// actor 회전
	if (!m_Rotate)
		return;
	rot = m_Rotate->Rotate(GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);
	SetRotation(rot);
}