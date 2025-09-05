#include "pch.h"
#include "IObjectClass.h"
#include "ObjectFactoryClass.h"
#include "ObjectManagerClass.h"

bool Object::ObjectManagerClass::IsInitialize = false;

Object::ObjectManagerClass::ObjectManagerClass()
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

Object::IObjectClass* Object::ObjectManagerClass::GetGameObject(UINT idx) const
{
	assert(idx < m_ObjectList.size());
	return m_ObjectList[idx].get();
}

void Object::ObjectManagerClass::Frame(const System::InputClass* input, Text::TextManagerClass* texts, Sound::SoundManagerClass* sounds, float frame_time, bool IsESCPopupWindowActivated)
{
	UINT cnt = static_cast<UINT>(m_ObjectList.size());
	for (UINT i = 0; i < cnt; ++i)
		m_ObjectList[i]->Update(input, this, texts, sounds, frame_time, IsESCPopupWindowActivated);
}

UINT Object::ObjectManagerClass::Load(ID ObjectID, Graphic::Model::ID ModelID)
{
	std::unique_ptr<IObjectClass> obj = std::move(m_Loader->Load(ObjectID, ModelID));
	assert(obj);

	m_ObjectList.push_back(std::move(obj));

	m_ModelMask |= (1 << static_cast<UINT>(ModelID));

#ifdef _DEBUG
	m_ModelMask |= (1 << static_cast<UINT>(Graphic::Model::ID::COLLISION));
#endif

	return static_cast<UINT>(m_ObjectList.size() - 1);
}

void Object::ObjectManagerClass::Release()
{
	m_ModelMask = 0;
	m_CameraIdx = 0;
	m_ObjectList.clear();
}
