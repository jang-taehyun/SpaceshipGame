#include "pch.h"
#include "IObjectClass.h"
#include "ActorClass.h"
#include "CollisionClass.h"
#include "MoveClass.h"
#include "RotateClass.h"
#include "ObjectManagerClass.h"

bool Object::ObjectManagerClass::IsInitialize = false;

Object::ObjectManagerClass::ObjectManagerClass()
{
	// ActorManagerClass 인스턴스가 이미 존재하는지 검사 //
	assert(!IsInitialize);
	IsInitialize = true;
}

Object::ObjectManagerClass::~ObjectManagerClass()
{
	IsInitialize = false;
}

Object::IObjectClass* Object::ObjectManagerClass::operator[](int idx) const
{
	assert(idx < m_Objects.size());
	return m_Objects[idx].get();
}

UINT Object::ObjectManagerClass::Load(const AffineInfo& ActorAffine, const AffineInfo& CollisionAffine, Graphic::Model::ID ModelID, Graphic::Shader::ID ShaderID)
{
	std::unique_ptr<IMoveClass> moveInst = std::make_unique<MoveClass>();
	std::unique_ptr<IRotateClass> rotateInst = std::make_unique<RotateClass>();
	std::unique_ptr<IObjectClass> collisionInst = std::make_unique<CollisionClass>(CollisionAffine);
	std::unique_ptr<IObjectClass> actorInst = std::make_unique<ActorClass>(ActorAffine, std::move(moveInst), std::move(rotateInst), std::move(collisionInst), ModelID, ShaderID);

	assert(actorInst);
	m_Objects.push_back(std::move(actorInst));

	m_ModelIDs |= (1 << static_cast<UINT>(ModelID));
	m_ShaderIDs |= (1 << static_cast<UINT>(ShaderID));

	return m_Objects.size() - 1;
}

void Object::ObjectManagerClass::Release()
{
	m_ModelIDs = 0;
	m_ShaderIDs = 0;
	m_PlayerIdx = 0;
	m_Objects.clear();
}
