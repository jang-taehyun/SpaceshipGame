#include "pch.h"
#include "ModelClass.h"
#include "ModelFactoryClass.h"
#include "ModelManagerClass.h"

bool Graphic::Model::ModelManagerClass::IsInitialize = false;

Graphic::Model::ModelManagerClass::ModelManagerClass() : m_CurrentModelMask(0), m_NeedShaderMask(0)
{
	assert(!IsInitialize);

	m_Factory = std::make_unique<Loader::ModelFactoryClass>();
	assert(m_Factory);

	IsInitialize = true;
}

Graphic::Model::ModelManagerClass::~ModelManagerClass()
{
	IsInitialize = false;
}

Graphic::Model::IModelClass* Graphic::Model::ModelManagerClass::GetModel(Graphic::Model::ID key) const
{
	std::map<Graphic::Model::ID, std::unique_ptr<IModelClass>>::const_iterator iter;
	
	iter = m_ModelList.find(key);
	if (m_ModelList.end() == iter)
		return nullptr;

	return iter->second.get();
}

void Graphic::Model::ModelManagerClass::Load(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT ModelMask)
{
	bool IsLoad = false, IsExist = false;
	ID id = ID::NONE;
	UINT flag = 0;
	std::unique_ptr<IModelClass> model = nullptr;
	std::map<Graphic::Model::ID, std::unique_ptr<IModelClass>>::iterator iter;

	for (UINT i = 0; i < ModelIDCount; ++i)
	{
		IsLoad = (ModelMask & (1 << i));
		IsExist = (m_CurrentModelMask & (1 << i));
		id = static_cast<ID>(i);

		// 로드를 해야하는데 map에 없는 경우
		if (IsLoad && !IsExist)
		{
			// instance 생성
			model = std::move(m_Factory->Load(hwnd, Device, DeviceContext, id));
			assert(model);

			// 필요한 shader ID, 현재 로드된 model ID 업데이트
			m_NeedShaderMask |= (1 << static_cast<UINT>(model->GetShaderID()));
			m_CurrentModelMask |= (1 << i);

			// map에 저장
			m_ModelList.insert(std::make_pair(id, std::move(model)));
		}
		// 해제해야 하는데 map에 있는 경우
		else if (!IsLoad && IsExist)
		{
			// instance 가져오기
			iter = m_ModelList.find(id);

			if (m_ModelList.end() != iter)
			{
				// 필요없는 shader ID 제거
				flag = ~(1 << static_cast<UINT>(iter->second->GetShaderID()));
				m_NeedShaderMask &= flag;

				// 필요없는 ModelClass instance 해제 //
				m_ModelList.erase(id);
			}

			// 현재 로드된 Model ID 제거
			flag = ~(1 << static_cast<UINT>(id));
			m_CurrentModelMask &= flag;
		}
	}


	// cube 모델 객체 생성 //
	// model 객체 생성
	// loader = std::make_unique<Loader::PTN_ModelLoaderClass>(ID::COLLISION);
	// model = std::make_unique<ModelClass<PTN_VertexType>>(hwnd, Device, DeviceContext, ID::COLLISION, loader);
	// assert(model);
}