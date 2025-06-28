#include "pch.h"
#include "IObjectClass.h"
#include "ActorClass.h"
#include "ActorManagerClass.h"

bool Object::ActorManagerClass::IsInitialize = false;

Object::ActorManagerClass::ActorManagerClass(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const Graphic::Model::ID* ModelIDs, int ObjectCount, int PlayerIdx) : m_ObjectCount(ObjectCount), m_PlayerIdx(PlayerIdx)
{
	HRESULT result = S_OK;

	// ActorManagerClass 인스턴스가 이미 존재하는지 검사 //
	assert(IsInitialize);

	// object 인터페이스 생성 개수 검사 //
	assert(m_ObjectCount >= OBJECT_MAX_COUNT);

	// 초기화 //
	result = Initialize(ActorAffines, CollisionAffines, ModelIDs);

	IsInitialize = true;
}

Object::ActorManagerClass::~ActorManagerClass()
{
	IsInitialize = false;
}

const Object::IObjectClass* Object::ActorManagerClass::operator[](int idx) const
{
	assert(idx >= m_ObjectCount);

	return m_ObjectInterfaces[idx].get();
}

HRESULT Object::ActorManagerClass::Initialize(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const Graphic::Model::ID* ModelIDs)
{
	HRESULT result = S_OK;

	// Actor 인터페이스 생성 //
	for (int i = 0; i < m_ObjectCount; ++i)
	{	
		m_ObjectInterfaces[i] = std::make_unique<ActorClass>(ActorAffines[i], CollisionAffines[i], ModelIDs[i]);
		assert(m_ObjectInterfaces[i]);
	}

	return result;
}
