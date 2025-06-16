#include "pch.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "ICollisionClass.h"
#include "CollisionClass.h"
#include "ActorClass.h"

ActorClass::ActorClass(const AffineInfo& affine, std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate, std::unique_ptr<ICollisionClass> collision, ModelIDs ModelID) : MoveableObjectClass(affine, std::move(move), std::move(rotate)), m_Collision(std::move(collision)), m_ModelID(ModelID) {}
ActorClass::ActorClass(const ActorClass& other) : MoveableObjectClass(other), m_Collision(other.m_Collision->Clone()), m_ModelID(other.m_ModelID) {}
ActorClass::ActorClass(ActorClass&& other) noexcept : MoveableObjectClass(std::move(other)), m_Collision(std::move(other.m_Collision)), m_ModelID(other.m_ModelID) {}

void ActorClass::Move(MoveState state, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 pos;

	// 최종적으로 계산된 position으로 교체 //
	// actor 이동
	if (!GetMoveObject())
		return;
	pos = GetMoveObject()->Move(GetPosition(), GetRotation(), state, frame_time, IsKeyDown);
	SetPosition(pos);

	// collision 이동
	if (!m_Collision)
		return;
	pos = GetMoveObject()->Move(m_Collision->GetPosition(), m_Collision->GetRotation(), state, frame_time, IsKeyDown);
	m_Collision->SetPosition(pos);
}

void ActorClass::Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 rot;

	// 최종적으로 계산된 rotation으로 교체 //
	// actor 회전
	if (!GetRotateObject())
		return;
	rot = GetRotateObject()->Rotate(GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);
	SetRotation(rot);

	// collision 회전
	if (!m_Collision)
		return;
	rot = GetRotateObject()->Rotate(m_Collision->GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);
	m_Collision->SetRotation(rot);
}

ActorClass& ActorClass::operator=(const ActorClass& other)
{
	if (this == &other)
		return *this;

	m_ModelID = other.m_ModelID;

	if (m_Collision)
		m_Collision.reset();

	m_Collision = other.m_Collision->Clone();

	return *this;
}

ActorClass& ActorClass::operator=(ActorClass&& other) noexcept
{
	if (this == &other)
		return *this;

	m_ModelID = other.m_ModelID;

	if (m_Collision)
		m_Collision.reset();

	m_Collision = std::move(other.m_Collision);

	return *this;
}

inline std::unique_ptr<IObjectClass> ActorClass::Clone() const
{
	return std::make_unique<ActorClass>(*this);
}
