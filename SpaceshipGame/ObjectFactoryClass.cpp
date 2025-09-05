#include "pch.h"
#include "ActorClass.h"
#include "CollisionClass.h"
#include "CameraClass.h"
#include "MoveClass.h"
#include "RotateClass.h"
#include "ObjectFactoryClass.h"

bool Object::ObjectFactoryClass::IsInitialize = false;

Object::ObjectFactoryClass::ObjectFactoryClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

	m_Creator.insert(std::make_pair(ID::ACTOR, CreateActor));
	m_Creator.insert(std::make_pair(ID::COLLISION, CreateCollision));
	m_Creator.insert(std::make_pair(ID::CAMERA, CreateCamera));
}

Object::ObjectFactoryClass::~ObjectFactoryClass()
{
	IsInitialize = false;
}

std::unique_ptr<Object::IObjectClass> Object::ObjectFactoryClass::Load(ID ObjectID, Graphic::Model::ID ModelID) const
{
	std::map<ID, std::function<std::unique_ptr<IObjectClass>(Graphic::Model::ID)>>::const_iterator iter;

	iter = m_Creator.find(ObjectID);
	assert(m_Creator.end() != iter);

	return iter->second(ModelID);
}

std::unique_ptr<Object::IObjectClass> Object::CreateActor(Graphic::Model::ID ModelID)
{
	assert(Graphic::Model::ID::NONE != ModelID);

	std::unique_ptr<IMoveClass> move = std::make_unique<MoveClass>();
	assert(move);

	std::unique_ptr<IRotateClass> rotate = std::make_unique<RotateClass>();
	assert(rotate);

	std::unique_ptr<IObjectClass> collision = std::make_unique<CollisionClass>();
	assert(collision);

	std::unique_ptr<IObjectClass> obj = std::make_unique<ActorClass>(std::move(move), std::move(rotate), std::move(collision), ModelID);
	assert(obj);

	return obj;
}

std::unique_ptr<Object::IObjectClass> Object::CreateCollision(Graphic::Model::ID ModelID)
{
	assert(Graphic::Model::ID::NONE != ModelID);

	std::unique_ptr<IObjectClass> obj = std::make_unique<CollisionClass>();
	assert(obj);

	return obj;
}

std::unique_ptr<Object::IObjectClass> Object::CreateCamera(Graphic::Model::ID ModelID)
{
	std::unique_ptr<IMoveClass> move = std::make_unique<MoveClass>();
	assert(move);

	std::unique_ptr<IRotateClass> rotate = std::make_unique<RotateClass>();
	assert(rotate);

	std::unique_ptr<IObjectClass> obj = std::make_unique<CameraClass>(std::move(move), std::move(rotate));
	assert(obj);

	return obj;
}
