#include "pch.h"
#include <fstream>
#include "ModelLoaderClass.h"
#include "ModelClass.h"

ModelClass::ModelClass() {}
ModelClass::ModelClass(const ModelClass& other) {}
ModelClass::~ModelClass() {}

HRESULT ModelClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& TextureFileName, const tstring& ModelFileName)
{
	m_Loader = new ModelLoaderClass;
	if (!m_Loader)
	{
		return E_FAIL;
	}

	if (FAILED(LoadModel(ModelFileName)))
	{
		return E_FAIL;
	}

	m_Loader->ReleaseData();
	m_Loader = nullptr;

	if (FAILED(InitializeBuffers(Device)))
	{
		return E_FAIL;
	}

	return LoadTexture(Device, DeviceContext, TextureFileName);
}

HRESULT ModelClass::Initialize(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<tstring>& TextureFileNames, const tstring& ModelFileName)
{
	m_Loader = new ModelLoaderClass;
	if (!m_Loader)
	{
		return E_FAIL;
	}

	if (FAILED(LoadModel(ModelFileName)))
	{
		return E_FAIL;
	}

	m_Loader->ReleaseData();
	m_Loader = nullptr;

	if (FAILED(InitializeBuffers(Device)))
	{
		return E_FAIL;
	}

	return LoadTexture(Device, DeviceContext, TextureFileNames);
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
	ReleaseTexture();
	ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* const& DeviceContext)
{
	// �������� ���� �׷��Ƚ� ���̽����ο� vertex buffer�� index buffer�� ���� //
	RenderBuffers(DeviceContext);

	// shader�� ���� ������
}

HRESULT ModelClass::LoadModel(const tstring& FileName)
{
	if (FAILED(m_Loader->LoadModel(FileName)))
	{
		return E_FAIL;
	}

	m_VertexCount = m_Loader->GetPositionCount();
	m_IndexCount = m_Loader->GetIndexCount();
	m_Vertices = new VertexType[m_Loader->GetFaceIndex()];
	if (!m_Vertices)
	{
		return E_FAIL;
	}

	if (FAILED(m_Loader->CopyModelData(m_Vertices)))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT ModelClass::InitializeBuffers(ID3D11Device* const& Device)
{
	// ���� ������ ���� //
	// vertex�� index ������ ����
	unsigned long* m_Indices = new unsigned long[m_IndexCount];
	if (!m_Indices)
	{
		return E_FAIL;
	}

	for (int i = 0; i < m_VertexCount; ++i)
	{
		m_Indices[i] = i;
	}

	// vertex buffer ���� //
	D3D11_BUFFER_DESC VertexBufferDesc;
	memset(&VertexBufferDesc, 0, sizeof(VertexBufferDesc));

	// vertex buffer ����
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// ���� �����͸� ����Ű�� subresource ���� �� ����
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = m_Vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// vertex buffer ����
	if (FAILED(Device->CreateBuffer(&VertexBufferDesc, &VertexData, &m_VertexBufer)))
	{
		return E_FAIL;
	}

	// index buffer ���� //
	D3D11_BUFFER_DESC IndexBufferDesc;
	memset(&IndexBufferDesc, 0, sizeof(IndexBufferDesc));

	// index buffer ����
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� subresource ���� �� ����
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = m_Indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// index buffer ����
	if (FAILED(Device->CreateBuffer(&IndexBufferDesc, &IndexData, &m_IndexBuffer)))
	{
		return E_FAIL;
	}

	// ���� ������, �ε��� ������ ����
	delete[] m_Vertices;
	m_Vertices = nullptr;

	delete[] m_Indices;
	m_Indices = nullptr;

	return S_OK;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	// offset(������)�� ���� �������� stride(����) ���� //
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler���� vertex buffer, index buffer Ȱ��ȭ //
	DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBufer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer���� �׸� object�� �⺻ ���� ���� //
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

HRESULT ModelClass::LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const tstring& FileName)
{
	// texture object ���� //
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return E_FAIL;
	}

	// targa �̹����� ���� ��� //
	return m_Texture->Initialize(Device, DeviceContext, FileName);
}

HRESULT ModelClass::LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<tstring>& FileNames)
{
	// texture object ���� //
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return E_FAIL;
	}

	// targa �̹����� ���� ��� //
	return m_Texture->Initialize(Device, DeviceContext, FileNames);
}

void ModelClass::ShutdownBuffers()
{
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = nullptr;
	}

	if (m_VertexBufer)
	{
		m_VertexBufer->Release();
		m_VertexBufer = nullptr;
	}
}

void ModelClass::ReleaseModel()
{
	if (m_Indices)
	{
		delete[] m_Indices;
		m_Indices = nullptr;
	}

	if (m_Vertices)
	{
		delete[] m_Vertices;
		m_Vertices = nullptr;
	}

	if (m_Loader)
	{
		m_Loader->ReleaseData();
		delete[] m_Loader;
		m_Loader = nullptr;
	}
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = nullptr;
	}
}