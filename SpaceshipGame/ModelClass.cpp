#include "pch.h"

#include "TextureClass.h"
#include "TextureArrayClass.h"
#include <fstream>

#include "ModelClass.h"

ModelClass::ModelClass() {}
ModelClass::ModelClass(const ModelClass& other) {}
ModelClass::~ModelClass() {}

bool ModelClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* TextureFileName, const char* ModelFileName)
{
	if (!LoadModel(ModelFileName))
	{
		return false;
	}

	if (!InitializeBuffers(Device))
	{
		return false;
	}

	return LoadTexture(Device, DeviceContext, TextureFileName);
}

bool ModelClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tTextureFileName, const char* ModelFileName)
{
	if (!LoadModel(ModelFileName))
	{
		return false;
	}

	if (!InitializeBuffers(Device))
	{
		return false;
	}

	return LoadTexture(Device, DeviceContext, tTextureFileName);
}

bool ModelClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tDefaultTextureFileName, const TCHAR* tColorTextureFileName, const char* ModelFileName)
{
	if (!LoadModel(ModelFileName))
	{
		return false;
	}

	if (!InitializeBuffers(Device))
	{
		return false;
	}

	return LoadTextures(Device, tDefaultTextureFileName, tColorTextureFileName);
}

bool ModelClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tDefaultTextureFileName, const TCHAR* tColorTextureFileName, const TCHAR* tAlphaMapTextureFilename, const char* ModelFileName)
{
	if (!LoadModel(ModelFileName))
	{
		return false;
	}

	if (!InitializeBuffers(Device))
	{
		return false;
	}

	return LoadTextures(Device, tDefaultTextureFileName, tColorTextureFileName, tAlphaMapTextureFilename);
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ReleaseTextures();

	ShutdownBuffers();

	ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* DeviceContext)
{
	// 렌더링을 위해 그래픽스 파이스라인에 vertex buffer와 index buffer를 설정 //
	RenderBuffers(DeviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_IndexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

ID3D11ShaderResourceView** ModelClass::GetTextures()
{
	return m_TextureArray->GetTextureArray();
}

bool ModelClass::InitializeBuffers(ID3D11Device* Device)
{
	// 정점 데이터 설정 //
	// 정점 배열, 인덱스 배열 생성
	VertexType* vertices = new VertexType[m_VertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[m_IndexCount];
	if (!indices)
	{
		return false;
	}

	// 데이터를 정점 배열, 인덱스 배열로 복사
	for (int i = 0; i < m_VertexCount; i++)
	{
		vertices[i].position = DirectX::XMFLOAT3(m_Model[i].x, m_Model[i].y, m_Model[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(m_Model[i].tu, m_Model[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(m_Model[i].nx, m_Model[i].ny, m_Model[i].nz);

		indices[i] = i;
	}

	// vertex buffer 생성 //
	D3D11_BUFFER_DESC VertexBufferDesc;
	memset(&VertexBufferDesc, 0, sizeof(VertexBufferDesc));

	// vertex buffer 설정
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// 정점 데이터를 가르키는 subresource 생성 및 설정
	D3D11_SUBRESOURCE_DATA VertexData;
	VertexData.pSysMem = vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// vertex buffer 생성
	if (FAILED(Device->CreateBuffer(&VertexBufferDesc, &VertexData, &m_VertexBufer)))
	{
		return false;
	}

	// index buffer 생성 //
	D3D11_BUFFER_DESC IndexBufferDesc;
	memset(&IndexBufferDesc, 0, sizeof(IndexBufferDesc));

	// index buffer 설정
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가르키는 subresource 생성 및 설정
	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// index buffer 생성
	if (FAILED(Device->CreateBuffer(&IndexBufferDesc, &IndexData, &m_IndexBuffer)))
	{
		return false;
	}

	// 정점 데이터, 인덱스 데이터 해제
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
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

void ModelClass::RenderBuffers(ID3D11DeviceContext* DeviceContext)
{
	// offset(오프셋)과 정점 데이터의 stride(단위) 설정 //
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// input assembler에서 vertex buffer, index buffer 활성화 //
	DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBufer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer에서 그릴 object의 기본 도형 설정 //
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* FileName)
{
	// texture object 생성 //
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// targa 이미지를 쓰는 경우 //
	return m_Texture->Initialize(Device, DeviceContext, FileName);
}

bool ModelClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const TCHAR* tFileName)
{
	// texture object 생성 //
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// targa 이미지를 쓰는 경우 //
	return m_Texture->Initialize(Device, DeviceContext, tFileName);
}

bool ModelClass::LoadTextures(ID3D11Device* Device, const TCHAR* tDefaultTextureFileName, const TCHAR* tColorTextureFileName)
{
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	if (!m_TextureArray->Initialize(Device, tDefaultTextureFileName, tColorTextureFileName))
	{
		return false;
	}

	return true;
}

bool ModelClass::LoadTextures(ID3D11Device* Device, const TCHAR* tDefaultTextureFileName, const TCHAR* tColorTextureFileName, const TCHAR* tAlphaMapTextureFilename)
{
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	if (!m_TextureArray->Initialize(Device, tDefaultTextureFileName, tColorTextureFileName, tAlphaMapTextureFilename))
	{
		return false;
	}

	return true;
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

void ModelClass::ReleaseTextures()
{
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = nullptr;
	}
}

bool ModelClass::LoadModel(const char* FileName)
{
	std::ifstream FileIn;

	// model file 열기 //
	FileIn.open(FileName);
	if (FileIn.fail())
	{
		return false;
	}

	// vertex count의 값까지 file의 내용을 read //
	char input = 0;
	FileIn.get(input);
	while (':' != input)
		FileIn.get(input);

	// vertex count의 값을 멤버 변수(m_VertexCount)에 복사 //
	FileIn >> m_VertexCount;

	// index의 수를 정점의 수와 같게 설정 //
	m_IndexCount = m_VertexCount;

	// 읽어 들인 정점의 개수를 사용해 model을 만들기 //
	m_Model = new ModelType[m_VertexCount];
	if (!m_Model)
	{
		return false;
	}

	// 데이터의 시작 부분까지 file을 read
	FileIn.get(input);
	while (':' != input)
		FileIn.get(input);
	FileIn.get(input);
	FileIn.get(input);

	// vertex 데이터를 read
	for (int i = 0; i < m_VertexCount; i++)
	{
		FileIn >> m_Model[i].x >> m_Model[i].y >> m_Model[i].z;
		FileIn >> m_Model[i].tu >> m_Model[i].tv;
		FileIn >> m_Model[i].nx >> m_Model[i].ny >> m_Model[i].nz;
	}

	// model file 닫기 //
	FileIn.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_Model)
	{
		delete[] m_Model;
		m_Model = nullptr;
	}
}
