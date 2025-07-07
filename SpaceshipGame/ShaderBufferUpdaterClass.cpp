#include "pch.h"
#include "MLC_ShaderClass.h"
#include "ShaderBufferUpdaterClass.h"

bool Graphic::Shader::ShaderBufferUpdaterClass::IsInitialize = false;

Graphic::Shader::ShaderBufferUpdaterClass::ShaderBufferUpdaterClass()
{
	assert(!IsInitialize);
	IsInitialize = true;

	m_UpdaterList.insert(std::make_pair(ID::DEFAULT_SPACESHIP, UpdateDefaultSpaceshipShaderBuffer));
}

Graphic::Shader::ShaderBufferUpdaterClass::~ShaderBufferUpdaterClass()
{
	IsInitialize = false;
}

void Graphic::Shader::ShaderBufferUpdaterClass::Update(ID3D11DeviceContext* DeviceContext, const BuffersData& data, ID ShaderID, IShaderClass* shader) const
{
	std::map<ID, std::function<void(ID3D11DeviceContext*, const BuffersData&, IShaderClass*)>>::const_iterator iter;

	iter = m_UpdaterList.find(ShaderID);
	assert(m_UpdaterList.end() != iter);

	iter->second(DeviceContext, data, shader);
}

void Graphic::Shader::UpdateDefaultSpaceshipShaderBuffer(ID3D11DeviceContext* DeviceContext, const BuffersData& data, IShaderClass* shader)
{
	MLC_ShaderBuffers buffers;

	buffers.transform = data.transform;
	buffers.light = data.light;
	buffers.camera = data.camera;
	
	static_cast<MLC_ShaderClass*>(shader)->SetShaderBuffers(DeviceContext, buffers);
}
