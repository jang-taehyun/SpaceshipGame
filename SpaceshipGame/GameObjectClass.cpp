#include "pch.h"
#include "CollisionClass.h"
#include "GameObjectClass.h"

Object::GameObjectClass::GameObjectClass(Graphic::Model::ID ModelID, std::unique_ptr<IObjectClass> collision)
	: m_Collision(std::move(collision)),
	m_ModelID(ModelID)
{}

Object::GameObjectClass::GameObjectClass(const GameObjectClass& other)
	: ObjectClass(other),
	m_ModelID(other.m_ModelID),
	m_Collision(other.m_Collision->Clone())
{}

Object::GameObjectClass::GameObjectClass(GameObjectClass&& other) noexcept
	: ObjectClass(std::move(other)),
	m_ModelID(other.m_ModelID),
	m_Collision(std::move(other.m_Collision))
{}

Object::GameObjectClass& Object::GameObjectClass::operator=(const GameObjectClass& other)
{
	if (this == &other)
		return *this;

	if (m_Collision)
		m_Collision.reset();

	m_ModelID = other.m_ModelID;
	m_Collision = other.m_Collision->Clone();

	ObjectClass::operator=(other);

	return *this;
}

Object::GameObjectClass& Object::GameObjectClass::operator=(GameObjectClass&& other) noexcept
{
	if (this == &other)
		return *this;

	if (m_Collision)
		m_Collision.reset();

	m_ModelID = other.m_ModelID;
	m_Collision = std::move(other.m_Collision);

	ObjectClass::operator=(std::move(other));

	return *this;
}
