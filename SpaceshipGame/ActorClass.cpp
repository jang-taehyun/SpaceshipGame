#include "pch.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "CollisionClass.h"
#include "ActorClass.h"
#include "CameraClass.h"

Object::ActorClass::ActorClass(std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate, std::unique_ptr<IObjectClass> collision, Graphic::Model::ID ModelID)
	: GameObjectClass(ModelID, std::move(collision)),
	m_Move(std::move(move)),
	m_Rotate(std::move(rotate))
{}

Object::ActorClass::ActorClass(const ActorClass& other)
	: GameObjectClass(other),
	m_Move(other.m_Move->Clone()),
	m_Rotate(other.m_Rotate->Clone())
{}

Object::ActorClass::ActorClass(ActorClass&& other) noexcept
	: GameObjectClass(std::move(other)),
	m_Move(std::move(other.m_Move)),
	m_Rotate(std::move(other.m_Rotate))
{}

void Object::ActorClass::Move(MoveState state, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 pos;

	// 최종적으로 계산된 position으로 교체 //
	// actor 이동
	if (!m_Move)
		return;
	pos = m_Move->Move(GetPosition(), GetRotation(), state, frame_time, IsKeyDown);
	SetPosition(pos);

	// collision 이동
	if (GetCollision())
		GetCollision()->SetPosition(pos);
}

void Object::ActorClass::Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown)
{
	DirectX::XMFLOAT4 rot;

	// 최종적으로 계산된 rotation으로 교체 //
	// actor 회전
	if (!m_Rotate)
		return;
	rot = m_Rotate->Rotate(GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);
	SetRotation(rot);

	// collision 회전
	if (GetCollision())
		GetCollision()->SetRotation(rot);
}

Object::ActorClass& Object::ActorClass::operator=(const ActorClass& other)
{
	if (this == &other)
		return *this;

	if (m_Move)
		m_Move.reset();
	if (m_Rotate)
		m_Rotate.reset();

	m_Move = other.m_Move->Clone();
	m_Rotate = other.m_Rotate->Clone();

	GameObjectClass::operator=(other);

	return *this;
}

Object::ActorClass& Object::ActorClass::operator=(ActorClass&& other) noexcept
{
	if (this == &other)
		return *this;

	if (m_Move)
		m_Move.reset();
	if (m_Rotate)
		m_Rotate.reset();

	m_Move = std::move(other.m_Move);
	m_Rotate = std::move(other.m_Rotate);

	GameObjectClass::operator=(std::move(other));

	return *this;
}
