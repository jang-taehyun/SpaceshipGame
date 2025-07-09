#pragma once

#include "pch.h"
#include "ModelLoaderClass.h"
#include "ModelClass.h"

template<typename VertexType>
Graphic::Model::ModelClass<VertexType>::ModelClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, ID ModelID, Shader::ID ShaderID, Loader::IModelLoaderClass* loader) : m_ModelID(ModelID), m_ShaderID(ShaderID)
{
	HRESULT result = Initialize(hwnd, Device, DeviceContext, loader);
}

template<typename VertexType>
Graphic::Model::ModelClass<VertexType>::ModelClass(const ModelClass& other)
	: m_ModelID(other.m_ModelID), m_ShaderID(other.m_ShaderID), m_MeshCount(other.m_MeshCount),
	m_VertexBuffer(other.m_VertexBuffer), m_IndexBuffer(other.m_IndexBuffer),
	m_MeshesVertexCount(other.m_MeshesVertexCount), m_MeshesIndexCount(other.m_MeshesIndexCount),
	m_Materials(other.m_Materials), m_WorldMatrix(other.m_WorldMatrix), m_ModelOBB(other.m_ModelOBB)
{
	// instance buffer 복사
	other.m_InstanceBuffer.CopyTo(m_InstanceBuffer.GetAddressOf());
}

template<typename VertexType>
Graphic::Model::ModelClass<VertexType>::ModelClass(ModelClass&& other) noexcept
	: m_ModelID(other.m_ModelID), m_ShaderID(other.m_ShaderID), m_MeshCount(other.m_MeshCount),
	m_VertexBuffer(std::move(other.m_VertexBuffer)), m_IndexBuffer(std::move(other.m_IndexBuffer)),
	m_MeshesVertexCount(std::move(other.m_MeshesVertexCount)), m_MeshesIndexCount(std::move(other.m_MeshesIndexCount)),
	m_Materials(std::move(other.m_Materials)), m_WorldMatrix(std::move(other.m_WorldMatrix)), m_ModelOBB(other.m_ModelOBB),
	m_InstanceBuffer(std::move(other.m_InstanceBuffer))
{}

template<typename VertexType>
Graphic::Model::ModelClass<VertexType>& Graphic::Model::ModelClass<VertexType>::operator=(const ModelClass& other)
{
	if (this == &other)
		return *this;

	// 멤버 변수 모두 해제 //
	m_VertexBuffer.clear();
	m_IndexBuffer.clear();
	m_MeshesVertexCount.clear();
	m_MeshesIndexCount.clear();
	m_Materials.clear();
	m_WorldMatrix.clear();

	// ID 값, mesh의 개수, OBB 박스 복사
	m_ModelID = other.m_ModelID;
	m_ShaderID = other.m_ShaderID;
	m_MeshCount = other.m_MeshCount;
	m_ModelOBB = other.m_ModelOBB;

	// vertex buffer, index buffer, instance buffer, material 복사
	m_VertexBuffer = other.m_VertexBuffer;
	m_IndexBuffer = other.m_IndexBuffer;
	other.m_InstanceBuffer.CopyTo(m_InstanceBuffer.ReleaseAndGetAddressOf());
	m_Materials = other.m_Materials;

	// 각 mesh의 vertex data의 개수, index data의 개수, world matrix 복사
	m_MeshesVertexCount = other.m_MeshesVertexCount;
	m_MeshesIndexCount = other.m_MeshesIndexCount;
	m_WorldMatrix = other.m_WorldMatrix;

	return *this;
}

template<typename VertexType>
Graphic::Model::ModelClass<VertexType>& Graphic::Model::ModelClass<VertexType>::operator=(ModelClass&& other) noexcept
{
	if (this == &other)
		return *this;

	// 멤버 변수 모두 해제 //
	m_VertexBuffer.clear();
	m_IndexBuffer.clear();
	m_MeshesVertexCount.clear();
	m_MeshesIndexCount.clear();
	m_Materials.clear();
	m_WorldMatrix.clear();
	m_InstanceBuffer.Reset();

	// ID 값, mesh의 개수, OBB 박스 복사
	m_ModelID = other.m_ModelID;
	m_ShaderID = other.m_ShaderID;
	m_MeshCount = other.m_MeshCount;
	m_ModelOBB = other.m_ModelOBB;

	// vertex buffer, index buffer, instance buffer, material 이동
	m_VertexBuffer = std::move(other.m_VertexBuffer);
	m_IndexBuffer = std::move(other.m_IndexBuffer);
	m_InstanceBuffer = std::move(other.m_InstanceBuffer);
	m_Materials = std::move(other.m_Materials);

	// 각 mesh의 vertex data의 개수, index data의 개수, world matrix 이동
	m_MeshesVertexCount = std::move(other.m_MeshesVertexCount);
	m_MeshesIndexCount = std::move(other.m_MeshesIndexCount);
	m_WorldMatrix = std::move(other.m_WorldMatrix);

	return *this;
}

template<typename VertexType>
HRESULT Graphic::Model::ModelClass<VertexType>::Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::IModelLoaderClass* loader)
{
	HRESULT result = S_OK;

	// model load //
	result = loader->Load(Device, DeviceContext);
	if (FAILED(result))
		return result;

	// vertex buffer, index buffer 생성 및 초기화 //
	result = InitializeBuffers(Device, DeviceContext, loader);
	if (FAILED(result))
		return result;

	// material 초기화 //
	InitializeMaterials(loader);

	return result;
}

template<typename VertexType>
HRESULT Graphic::Model::ModelClass<VertexType>::InitializeBuffers(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::IModelLoaderClass* loader)
{
	HRESULT result = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;		// vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;		// index buffer

	D3D11_BUFFER_DESC VertexBufferDesc = {};				// vertex buffer의 설정 정보
	D3D11_SUBRESOURCE_DATA VertexData = {};					// vertex 데이터를 가르키는 subresource 설정 정보
	D3D11_BUFFER_DESC IndexBufferDesc = {};					// index buffer의 설정 정보
	D3D11_SUBRESOURCE_DATA IndexData = {};					// index 데이터를 가르키는 subresource 설정 정보
	D3D11_BUFFER_DESC InstanceBufferDesc = {};				// instance buffer의 설정 정보

	std::vector<std::vector<VertexType>> VerticeDatas;		// 각 mesh의 vertex 데이터들
	std::vector<std::vector<ULONG>> IndicesDatas;			// 각 mesh의 index 데이터들

	// loader에서 로드한 데이터들 가져오기 //
	m_MeshCount = loader->GetMeshCount();
	VerticeDatas = static_cast<Loader::ModelLoaderClass<VertexType>*>(loader)->MoveVerticesDatas();
	IndicesDatas = loader->MoveIndicesDatas();
	m_ModelOBB = loader->GetModelOBB();

	//  vertex 데이터, index 데이터를 이용해 vertex buffer, index buffer 생성 //
	for(UINT i=0; i< m_MeshCount; ++i)
	{
		// vertex buffer 생성 //
		// vertex buffer 설정
		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(VertexType) * VerticeDatas[i].size());
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		// 정점 데이터를 가르키는 subresource 생성 및 설정
		VertexData.pSysMem = VerticeDatas[i].data();
		VertexData.SysMemPitch = 0;
		VertexData.SysMemSlicePitch = 0;

		// vertex buffer 생성
		result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, VertexBuffer.GetAddressOf());
		assert(SUCCEEDED(result));

		// index buffer 생성 //
		// index buffer 설정
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(ULONG) * IndicesDatas[i].size());
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;

		// index 데이터를 가르키는 subresource 생성 및 설정
		IndexData.pSysMem = IndicesDatas[i].data();
		IndexData.SysMemPitch = 0;
		IndexData.SysMemSlicePitch = 0;

		// index buffer 생성
		result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, IndexBuffer.GetAddressOf());
		assert(SUCCEEDED(result));

		m_MeshesVertexCount.push_back(static_cast<ULONG>(VerticeDatas[i].size()));
		m_MeshesIndexCount.push_back(static_cast<ULONG>(IndicesDatas[i].size()));
		m_VertexBuffer.push_back(std::move(VertexBuffer));
		m_IndexBuffer.push_back(std::move(IndexBuffer));
	}

	// instance buffer 생성 //
	// instance buffer 설정
	InstanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	InstanceBufferDesc.ByteWidth = static_cast<UINT>(sizeof(InstanceBufferType) * MAX_INSTANCE_COUNT);
	InstanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	InstanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	InstanceBufferDesc.MiscFlags = 0;
	InstanceBufferDesc.StructureByteStride = 0;

	// instance buffer 생성
	result = Device->CreateBuffer(&InstanceBufferDesc, nullptr, m_InstanceBuffer.GetAddressOf());
	assert(SUCCEEDED(result));

	return result;
}

template<typename VertexType>
void Graphic::Model::ModelClass<VertexType>::InitializeMaterials(Loader::IModelLoaderClass* loader)
{
	m_Materials = loader->MoveMaterialsDatas();
}

template<typename VertexType>
void Graphic::Model::ModelClass<VertexType>::UpdateInstanceBuffer(ID3D11DeviceContext* DeviceContext)
{
	HRESULT result = S_OK;
	D3D11_MAPPED_SUBRESOURCE MappedResource;			// lock
	InstanceBufferType* DataPtr = nullptr;				// buffer의 포인터

	// instance buffer의 내용을 CPU가 쓸 수 있도록 잠금 //
	result = DeviceContext->Map(m_InstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	assert(SUCCEEDED(result));

	// instance buffer의 데이터에 대한 포인터를 가져오기 //
	DataPtr = static_cast<InstanceBufferType*>(MappedResource.pData);

	// instance buffer에 데이터 복사
	std::copy(m_WorldMatrix.begin(), m_WorldMatrix.end(), DataPtr);

	// instance buffer의 잠금을 풀어 GPU에 반영
	DeviceContext->Unmap(m_InstanceBuffer.Get(), 0);

	// GPU에 반영되었으므로, 현재까지 저장된 world matrix를 모두 해제
	m_WorldMatrix.clear();
}

template<typename VertexType>
void Graphic::Model::ModelClass<VertexType>::RenderMesh(ID3D11DeviceContext* DeviceContext, UINT MeshIdx)
{
	assert(MeshIdx < m_MeshCount);
	assert(m_WorldMatrix.size() <= MAX_INSTANCE_COUNT);

	// offset(오프셋), 정점 데이터의 stride(단위), buffer 설정 //
	ID3D11Buffer* buffers[2] = { m_VertexBuffer[MeshIdx].Get(), m_InstanceBuffer.Get()};
	UINT stride[2] = { static_cast<UINT>(sizeof(VertexType)), static_cast<UINT>(sizeof(DirectX::XMFLOAT4X4)) };
	UINT offset[2] = { 0, 0 };

	// input assembler에서 vertex buffer, instance buffer, index buffer 활성화 //
	DeviceContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
	DeviceContext->IASetIndexBuffer(m_IndexBuffer[MeshIdx].Get(), DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer에서 그릴 object의 기본 도형 설정 //
	DeviceContext->IASetPrimitiveTopology((m_ModelID == ID::COLLISION ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}