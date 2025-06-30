#include "pch.h"
#include "ModelLoaderClass.h"
#include "ModelClass.h"

template<typename VertexType>
Graphic::Model::ModelClass<VertexType>::ModelClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, ID ModelID, Loader::ModelLoaderClass<VertexType>* loader) : m_ModelID(ModelID)
{
	HRESULT result = Initialize(hwnd, Device, DeviceContext, loader);
}

template<typename VertexType>
HRESULT Graphic::Model::ModelClass<VertexType>::Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::ModelLoaderClass<VertexType>* loader)
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
	InitializeMaterials(ModelLoader);

	return result;
}

template<typename VertexType>
HRESULT Graphic::Model::ModelClass<VertexType>::InitializeBuffers(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::ModelLoaderClass<VertexType>* loader)
{
	HRESULT result = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;		// vertex buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;		// index buffer

	D3D11_BUFFER_DESC VertexBufferDesc = {};				// vertex buffer의 설정 정보
	D3D11_SUBRESOURCE_DATA VertexData = {};					// vertex 데이터를 가르키는 subresource 설정 정보
	D3D11_BUFFER_DESC IndexBufferDesc = {};					// index buffer의 설정 정보
	D3D11_SUBRESOURCE_DATA IndexData = {};					// index 데이터를 가르키는 subresource 설정 정보

	std::vector<std::vector<VertexType>> VerticeDatas;		// 각 mesh의 vertex 데이터들
	std::vector<std::vector<ULONG>> IndicesDatas;			// 각 mesh의 index 데이터들

	// loader에서 로드한 데이터들 가져오기 //
	m_MeshCount = loader->GetMeshCount();
	VerticeDatas = loader->MoveVerticesDatas();
	IndicesDatas = loader->MoveIndicesDatas();

	//  vertex 데이터, index 데이터를 이용해 vertex buffer, index buffer 생성 //
	for(int i=0; i< m_MeshCount; ++i)
	{
		// vertex buffer 생성 //
		// vertex buffer 설정
		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = sizeof(VertexType) * VerticeDatas[i].size();
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
		IndexBufferDesc.ByteWidth = sizeof(ULONG) * IndicesDatas[i].size();
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

		m_MeshesVertexCount.push_back(VerticeDatas[i].size());
		m_MeshesIndexCount.push_back(IndicesDatas[i].size());
		m_VertexBuffer.push_back(std::move(VertexBuffer));
		m_IndexBuffer.push_back(std::move(IndexBuffer));
	}

	return result;
}

template<typename VertexType>
void Graphic::Model::ModelClass<VertexType>::InitializeMaterials(Loader::ModelLoaderClass<VertexType>* loader)
{
	m_Materials = loader->MoveMaterialsDatas();
}

template<typename VertexType>
void Graphic::Model::ModelClass<VertexType>::RenderMesh(ID3D11DeviceContext* DeviceContext, int MeshIdx)
{
	assert(MeshIdx < m_MeshCount);

	// offset(오프셋)과 정점 데이터의 stride(단위) 설정 //
	ULONG stride = static_cast<ULONG>(sizeof(VertexType));
	ULONG offset = 0;

	// input assembler에서 vertex buffer, index buffer 활성화 //
	DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer[MeshIdx]->GetAddressOf(), &stride, &offset);
	DeviceContext->IASetIndexBuffer(m_IndexBuffer[MeshIdx]->Get(), DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer에서 그릴 object의 기본 도형 설정 //
	DeviceContext->IASetPrimitiveTopology((m_ModelID == ID::COLLISION ? D3D11_PRIMITIVE_TOPOLOGY_LINELIST : D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
}