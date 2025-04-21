#include "pch.h"
#include <fstream>
#include "ShaderClass.h"
#include "ModelClass.h"

static ErrorContent e;

ModelClass::ModelClass(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ModelClass constructor");

	result = Initialize(hwnd, Device, DeviceContext, info);
	if (FAILED(result))
	{
		Shutdown();
		throw e;
	}
}

ModelClass::~ModelClass()
{
	Shutdown();
}

HRESULT ModelClass::Initialize(const HWND& hwnd, ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const ModelInfo& info)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ModelClass Initialize()");

	// model load //
	result = LoadModel(info.ModelFileName);
	if (FAILED(result))
	{
		e.contents = _T("model load 실패");
		e.errorCode = result;
		return result;
	}

	// vertex buffer, index buffer 생성 및 초기화 //
	result = InitializeBuffers(Device);
	if (FAILED(result))
	{
		e.contents = _T("vertex buffer, index buffer 생성 및 초기화 실패");
		e.errorCode = result;
		return result;
	}

	// texture load //
	result = LoadTexture(Device, DeviceContext, info.TextureFileNames);
	if (FAILED(result))
	{
		e.contents = _T("texture load 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
	ReleaseTexture();
	ReleaseModel();
}

HRESULT ModelClass::Render(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ModelClass Render()");

	// 렌더링을 위해 그래픽스 파이스라인에 vertex buffer와 index buffer를 설정 //
	SetBuffers(DeviceContext);

	// shader를 통해 렌더링
	result = RenderShader(DeviceContext, transform, light, camera);
	if (FAILED(result))
	{
		e.contents = _T("shader 렌더링 실패");
		e.errorCode = result;
		return result;
	}

	return result;
}

HRESULT ModelClass::LoadModel(const std::wstring& FileName)
{
	HRESULT result = S_OK;
	std::ifstream FileIn;
	char input = 0;

	// 에러 메세지 초기화 //
	e.title = _T("ModelClass LoadModel()");

	// model file 열기 //
	FileIn.open(FileName);
	if (FileIn.fail())
	{
		e.contents = _T("model file 열기 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// vertex count의 값까지 file의 내용을 read //
	FileIn.get(input);
	while (':' != input)
		FileIn.get(input);

	// vertex count의 값을 멤버 변수(m_VertexCount)에 복사 //
	FileIn >> m_VertexCount;

	// index의 수를 정점의 수와 같게 설정 //
	m_IndexCount = m_VertexCount;

	// 읽어 들인 정점의 개수를 사용해 model을 만들기 //
	m_Model = new ModelType[m_VertexCount];
	if(!m_Model)
	{
		e.contents = _T("model 데이터 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
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

	return result;
}

HRESULT ModelClass::InitializeBuffers(ID3D11Device* const& Device)
{
	HRESULT result = S_OK;
	VertexType* vertices = nullptr;			// 정점 데이터
	unsigned long* indices = nullptr;		// 인덱스 데이터
	D3D11_BUFFER_DESC VertexBufferDesc;		// vertex buffer의 설정 정보
	D3D11_SUBRESOURCE_DATA VertexData;		// 정점 데이터를 가르키는 subresource 설정 정보
	D3D11_BUFFER_DESC IndexBufferDesc;		// index buffer의 설정 정보
	D3D11_SUBRESOURCE_DATA IndexData;		// 인덱스 데이터를 가르키는 subresource 설정 정보

	// 에러 메세지 및 구조체 초기화 //
	e.title = _T("ModelClass InitializeBuffers()");
	memset(&VertexBufferDesc, 0, sizeof(VertexBufferDesc));
	memset(&IndexBufferDesc, 0, sizeof(IndexBufferDesc));

	// 정점 데이터 설정 //
	// 정점 배열, 인덱스 배열 생성
	vertices = new VertexType[m_VertexCount];
	if (!vertices)
	{
		e.contents = _T("정점 배열 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	indices = new unsigned long[m_IndexCount];
	if (!indices)
	{
		if (vertices)
		{
			delete[] vertices;
			vertices = nullptr;
		}

		e.contents = _T("인덱스 배열 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
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
	// vertex buffer 설정
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;

	// 정점 데이터를 가르키는 subresource 생성 및 설정
	VertexData.pSysMem = vertices;
	VertexData.SysMemPitch = 0;
	VertexData.SysMemSlicePitch = 0;

	// vertex buffer 생성
	result = Device->CreateBuffer(&VertexBufferDesc, &VertexData, &m_VertexBuffer);
	if (FAILED(result))
	{
		if (indices)
		{
			delete[] indices;
			indices = nullptr;
		}
		if (vertices)
		{
			delete[] vertices;
			vertices = nullptr;
		}

		e.contents = _T("vertex buffer 생성 실패");
		e.errorCode = result;
		return result;
	}

	// index buffer 생성 //
	// index buffer 설정
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가르키는 subresource 생성 및 설정
	IndexData.pSysMem = indices;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;

	// index buffer 생성
	result = Device->CreateBuffer(&IndexBufferDesc, &IndexData, &m_IndexBuffer);
	if (FAILED(result))
	{
		if (indices)
		{
			delete[] indices;
			indices = nullptr;
		}
		if (vertices)
		{
			delete[] vertices;
			vertices = nullptr;
		}

		e.contents = _T("index buffer 생성 실패");
		e.errorCode = result;
		return result;
	}

	// 정점 데이터, 인덱스 데이터 해제
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return result;
}

void ModelClass::SetBuffers(ID3D11DeviceContext* const& DeviceContext)
{
	// offset(오프셋)과 정점 데이터의 stride(단위) 설정 //
	UINT stride = static_cast<UINT>(sizeof(VertexType));
	UINT offset = 0;

	// input assembler에서 vertex buffer, index buffer 활성화 //
	DeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// vertex buffer에서 그릴 object의 기본 도형 설정 //
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

HRESULT ModelClass::LoadTexture(ID3D11Device* const& Device, ID3D11DeviceContext* const& DeviceContext, const std::vector<std::wstring>& FileNames)
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("ModelClass LoadTexture()");

	// texture object 생성 //
	m_Texture = new TextureClass(Device, DeviceContext, FileNames);
	if (!m_Texture)
	{
		e.contents = _T("texture 인스턴스 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	return result;
}

void ModelClass::ShutdownBuffers()
{
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = nullptr;
	}

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = nullptr;
	}
}

void ModelClass::ReleaseModel()
{
	if (m_Model)
	{
		delete[] m_Model;
		m_Model = nullptr;
	}
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		delete m_Texture;
		m_Texture = nullptr;
	}
}
