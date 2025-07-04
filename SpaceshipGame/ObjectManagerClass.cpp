#include "pch.h"
#include "IObjectClass.h"
#include "ObjectFactoryClass.h"
#include "ObjectManagerClass.h"

bool Object::ObjectManagerClass::IsInitialize = false;

Object::ObjectManagerClass::ObjectManagerClass() : m_PlayerIdx(0), m_ModelMask(0)
{
	assert(!IsInitialize);

	m_Loader = std::make_unique<ObjectFactoryClass>();
	assert(m_Loader);

	IsInitialize = true;
}

Object::ObjectManagerClass::~ObjectManagerClass()
{
	IsInitialize = false;
}

Object::IObjectClass* Object::ObjectManagerClass::operator[](int idx) const
{
	assert(idx < m_ObjectList.size());
	return m_ObjectList[idx].get();
}

Object::IObjectClass* Object::ObjectManagerClass::Load(ID ObjectID, Graphic::Model::ID ModelID)
{
	std::unique_ptr<IObjectClass> obj = std::move(m_Loader->Load(ObjectID, ModelID));
	assert(obj);

	m_ObjectList.push_back(std::move(obj));

	m_ModelMask |= (1 << static_cast<UINT>(ModelID));

	return m_ObjectList[m_ObjectList.size() - 1].get();
}

void Object::ObjectManagerClass::Release()
{
	m_ModelMask = 0;
	m_PlayerIdx = 0;
	m_ObjectList.clear();
}
