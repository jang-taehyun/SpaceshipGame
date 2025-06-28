#include "pch.h"
#include "IMoveClass.h"
#include "IRotateClass.h"
#include "MoveClass.h"
#include "RotateClass.h"
#include "MoveableObjectClass.h"

Object::MoveableObjectClass::MoveableObjectClass(const AffineInfo& affine, std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate) : GameObjectClass(affine), m_Move(std::move(move)), m_Rotate(std::move(rotate)) {}
Object::MoveableObjectClass::MoveableObjectClass(const MoveableObjectClass& other) : GameObjectClass(other), m_Move(std::move(other.GetMoveObject()->Clone())), m_Rotate(other.GetRotateObject()->Clone()) {}
Object::MoveableObjectClass::MoveableObjectClass(MoveableObjectClass&& other) noexcept : GameObjectClass(std::move(other)), m_Move(std::move(other.m_Move)), m_Rotate(std::move(other.m_Rotate)) {}

void Object::MoveableObjectClass::Move(MoveState state, float frame_time, bool IsKeyDown)
{
	if (!m_Move)
		return;

	// 최종적으로 계산된 position으로 교체 //
	DirectX::XMFLOAT4 pos;
	pos = m_Move->Move(GetPosition(), GetRotation(), state, frame_time, IsKeyDown);
	SetPosition(pos);
}

void Object::MoveableObjectClass::Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown)
{
	if (!m_Rotate)
		return;

	// 최종적으로 계산된 rotation으로 교체 //
	DirectX::XMFLOAT4 rot;
	rot = m_Rotate->Rotate(GetRotation(), MouseX, MouseY, frame_time, IsKeyDown);
	SetRotation(rot);
}

Object::MoveableObjectClass& Object::MoveableObjectClass::operator=(const MoveableObjectClass& other)
{
	// 자기 자신인지 검사 //
	if (this == &other)
		return *this;

	if (other.m_Move)
	{
		if (!(this->m_Move))
			this->m_Move = other.m_Move->Clone();
		else
			*m_Move = *(other.m_Move);
	}
	else
		this->m_Move.reset();

	if (other.m_Rotate)
	{
		if (!m_Rotate)
			m_Rotate = other.m_Rotate->Clone();
		else
			*m_Rotate = *(other.m_Rotate);
	}
	else
		this->m_Rotate.reset();

	return *this;
}

Object::MoveableObjectClass& Object::MoveableObjectClass::operator=(MoveableObjectClass&& other) noexcept
{
	// 자기 자신인지 검사 //
	if (this == &other)
		return *this;

	this->m_Move = std::move(other.m_Move);
	this->m_Rotate = std::move(other.m_Rotate);

	return *this;
}

inline std::unique_ptr<Object::IObjectClass> Object::MoveableObjectClass::Clone() const
{
	return std::make_unique<MoveableObjectClass>(*this);
}
