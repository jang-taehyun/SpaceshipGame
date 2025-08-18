#include "stdafx.h"
#include <fstream>
#include "SkyDomeClass.h"

bool SkyDomeClass::Initialize(ID3D11Device* device)
{
    // 스카이 돔 모델 정보를 읽어옵니다.
    if (!LoadSkyDomeModel("./data/skydome.txt"))
    {
        return false;
    }

    // 스카이 돔을 정점에 로드하고 렌더링을 위해 인덱스 버퍼를 로드합니다.
    if (!InitializeBuffers(device))
    {
        return false;
    }

    return true;
}

void SkyDomeClass::Render(ID3D11DeviceContext* deviceContext)
{
    // 스카이 돔을 렌더링 합니다.
    RenderBuffers(deviceContext);
}


bool SkyDomeClass::LoadSkyDomeModel(const char* filename)
{
    // 모델 파일을 엽니다.
    std::ifstream FileIn(filename, std::ios_base::in);

    // 파일을 열 수 없으면 종료합니다.
    if (FileIn.fail())
    {
        return false;
    }

    // 버텍스 카운트의 값까지 읽는다.
    char input = 0;
    FileIn.get(input);
    while (input != ':')
    {
        FileIn.get(input);
    }

    // 버텍스 카운트를 읽는다.
    FileIn >> m_vertexCount;

    // 인덱스의 수를 정점 수와 같게 설정합니다.
    m_indexCount = m_vertexCount;

    // 읽어 들인 정점 개수를 사용하여 모델을 만듭니다.
    m_model = std::make_unique<ModelType[]>(m_vertexCount);
    if (!m_model)
    {
        return false;
    }

    // 데이터의 시작 부분까지 읽는다.
    FileIn.get(input);
    while (input != ':')
    {
        FileIn.get(input);
    }
    FileIn.get(input);
    FileIn.get(input);

    // 버텍스 데이터를 읽습니다.
    for (int i = 0; i < m_vertexCount; i++)
    {
        FileIn >> m_model[i].x >> m_model[i].y >> m_model[i].z;
        FileIn >> m_model[i].tu >> m_model[i].tv;
        FileIn >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
    }

    // 모델 파일을 닫는다.
    FileIn.close();

    return true;
}

bool SkyDomeClass::InitializeBuffers(ID3D11Device* device)
{
    // 정점 배열을 만듭니다.
    std::unique_ptr<VertexType[]> vertices(std::make_unique<VertexType[]>(m_vertexCount));
    if (!vertices)
    {
        return false;
    }

    // 인덱스 배열을 만듭니다.
    std::unique_ptr<unsigned long[]> indices(std::make_unique<unsigned long[]>(m_indexCount));
    if (!indices)
    {
        return false;
    }

    // 정점 배열과 인덱스 배열을 데이터로 로드합니다.
    for (int i = 0; i < m_vertexCount; i++)
    {
        vertices[i].position = DirectX::XMFLOAT4(m_model[i].x, m_model[i].y, m_model[i].z, 1.f);
        indices[i] = i;
    }

    // 정점 버퍼의 구조체를 설정한다.
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices.get();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // 이제 마침내 정점 버퍼를 만듭니다.
    if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf())))
    {
        return false;
    }

    // 인덱스 버퍼의 구조체를 설정합니다.
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices.get();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // 인덱스 버퍼를 만듭니다.
    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf())))
    {
        return false;
    }

    return true;
}

void SkyDomeClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
    // 정점 버퍼 보폭 및 오프셋을 설정합니다.
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    // 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
    deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    // 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
    deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // 이 꼭지점 버퍼에서 렌더링되어야하는 프리미티브 유형을 설정합니다.이 경우에는 삼각형입니다.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
