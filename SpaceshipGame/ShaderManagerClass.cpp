#include "pch.h"
#include "IShaderClass.h"
#include "ShaderFactoryClass.h"
#include "ShaderBufferUpdaterClass.h"
#include "ShaderManagerClass.h"

bool Graphic::Shader::ShaderManagerClass::IsInitialize = false;

Graphic::Shader::ShaderManagerClass::ShaderManagerClass() : m_CurrentShaderMask(0)
{
	assert(!IsInitialize);

	m_Factory = std::make_unique<Loader::ShaderFactoryClass>();
	assert(m_Factory);

	m_Updater = std::make_unique<ShaderBufferUpdaterClass>();
	assert(m_Updater);

	IsInitialize = true;
}

Graphic::Shader::ShaderManagerClass::~ShaderManagerClass()
{
	IsInitialize = false;
}

Graphic::Shader::IShaderClass* Graphic::Shader::ShaderManagerClass::GetShader(ID key) const
{
	std::map<ID, std::unique_ptr<IShaderClass>>::const_iterator iter;

	iter = m_ShaderList.find(key);
	if (m_ShaderList.end() == iter)
		return nullptr;

	return iter->second.get();
}

void Graphic::Shader::ShaderManagerClass::Load(HWND hwnd, ID3D11Device* Device, UINT ShaderMask)
{
	bool IsLoad = false, IsExist = false;
	ID id = ID::NONE;
	UINT flag = 0;
	std::unique_ptr<IShaderClass> shader = nullptr;
	std::map<ID, std::unique_ptr<IShaderClass>>::iterator iter;

	for (UINT i = 0; i < ShaderIDCount+1; ++i)
	{
		IsLoad = (ShaderMask & (1 << i));
		IsExist = (m_CurrentShaderMask & (1 << i));
		id = static_cast<ID>(i);

		// 로드를 해야하는데 map에 없는 경우
		if (IsLoad && !IsExist)
		{
			// instance 생성
			shader = std::move(m_Factory->Load(hwnd, Device, id));
			assert(shader);

			// 현재 로드된 shader ID 업데이트
			m_CurrentShaderMask |= (1 << i);

			// map에 저장
			m_ShaderList.insert(std::make_pair(id, std::move(shader)));
		}
		// 해제해야 하는데 map에 있는 경우
		else if (!IsLoad && IsExist)
		{
			// instance 가져오기
			iter = m_ShaderList.find(id);

			// 필요없는 ShaderClass instance 해제 //
			if (m_ShaderList.end() != iter)
				m_ShaderList.erase(id);

			// 현재 로드된 Model ID 제거
			flag = ~(1 << static_cast<UINT>(id));
			m_CurrentShaderMask &= flag;
		}
	}
}

void Graphic::Shader::ShaderManagerClass::UpdateBuffer(ID3D11DeviceContext* DeviceContext, const BuffersData& data)
{
	std::map<ID, std::unique_ptr<IShaderClass>>::iterator iter;

	for (iter = m_ShaderList.begin(); iter != m_ShaderList.end(); ++iter)
		m_Updater->Update(DeviceContext, data, iter->first, iter->second.get());
}
