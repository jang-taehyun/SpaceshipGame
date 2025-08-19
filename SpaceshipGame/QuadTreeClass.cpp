#include "pch.h"
#include <fstream>
#include "TerrainShaderClass.h"
#include "CameraClass.h"
#include "QuadTreeClass.h"

Graphic::Terrain::QuadTreeClass::QuadTreeClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* HeightMap, const WCHAR* TextureFilename)
{
    Initialize(Device, DeviceContext, HeightMap, TextureFilename);
}

Graphic::Terrain::QuadTreeClass::~QuadTreeClass()
{
    Shutdown();
}

void Graphic::Terrain::QuadTreeClass::Shutdown()
{
    m_Texture.reset();
    m_Vertices.reset();

    ReleaseQuadTree(m_QuadTree.get());
    m_QuadTree.reset();
}

void Graphic::Terrain::QuadTreeClass::ReleaseQuadTree(QuadTree* node)
{
    if (!node)
        return;

    // QuadTree의 각 node 탐색
    for (int i = 0; i < 4; ++i)
    {
        if (node->nodes[i])
        {
            // 해당 node 탐색
            ReleaseQuadTree(node->nodes[i].get());

            // 탐색한 node 해제
            node->nodes[i].reset();
        }
    }

    // vertex buffer, index buffer 해제
    node->vertexBuffer.Reset();
    node->indexBuffer.Reset();
}

void Graphic::Terrain::QuadTreeClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const char* HeightMap, const WCHAR* TextureFilename)
{
    // texture 로드 //
    m_Texture = std::make_unique<Texture::TextureClass>(Device, DeviceContext, TextureFilename);
    assert(m_Texture);

    // 지형 데이터 로드 //
    if (!CreateVertexData(Device, HeightMap))
        assert(false);

    // 지형에 대한 quad tree 생성 //
    m_QuadTree = std::make_unique<QuadTree>();
    assert(m_QuadTree);

    CreateQuadTree(Device, 0, 0, m_TerrainHeight, m_TerrainWidth, m_QuadTree.get());

    // 전체 vertex 데이터 배열 해제 //
    m_Vertices.reset();
}

void Graphic::Terrain::QuadTreeClass::Render(ID3D11DeviceContext* DeviceContext, Shader::TerrainShaderClass* Shader, Object::CameraClass* Camera)
{
    RenderBuffers(DeviceContext, m_QuadTree.get(), Shader, Camera);
}

void Graphic::Terrain::QuadTreeClass::RenderBuffers(ID3D11DeviceContext* DeviceContext, QuadTree* node, Shader::TerrainShaderClass* Shader, Object::CameraClass* Camera)
{
    float x = (node->max_x + node->min_x) * 0.5f;
    float y = (node->max_y + node->min_y) * 0.5f;
    float z = (node->max_z + node->min_z) * 0.5f;
    UINT stride = sizeof(Model::PTN_VertexType);
    UINT offset = 0;

    // frustum 체크
    if (!Camera->IsRenderQuadTree(x, y, z, node->width * 0.5f))
        return;

    // child node를 체크하여 frustum volume에 포함되면 렌더링
    for (int i = 0; i < 4; ++i)
    {
        if (node->nodes[i])
            RenderBuffers(DeviceContext, (node->nodes[i]).get(), Shader, Camera);
    }

    if (node->vertexBuffer && node->indexBuffer)
    {
        // 렌더링 파이프라인에 vertex buffer, index buffer 세팅
        DeviceContext->IASetVertexBuffers(0, 1, (node->vertexBuffer).GetAddressOf(), &stride, &offset);
        DeviceContext->IASetIndexBuffer((node->indexBuffer).Get(), DXGI_FORMAT_R32_UINT, 0);
        DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // shader를 통해서 렌더링
        Shader->Render(DeviceContext, node->indexCount, m_Texture->GetTexture());
    }
}

bool Graphic::Terrain::QuadTreeClass::LoadHeightMap(const char* HeightMap)
{
    BITMAPFILEHEADER bitmapFileHeader = {};                     // BITMAP 파일의 헤더 정보
    BITMAPINFOHEADER bitmapInfoHeader = {};                     // BITMAP 정보의 헤더 정보
    int vertexCount = 0;                                        // 지형의 vertex 개수
    int imageSize = 0;                                          // BITMAP 이미지의 크기
    std::unique_ptr<unsigned char[]> bitmapImage = nullptr;     // BITMAP 이미지 데이터 배열
    int k = 0;                                                  // BITMAP 이미지 데이터 배열의 index
    int idx = 0;                                                // 지형의 vectex 배열의 index
    unsigned char height = 0;                                   // BITMAP 이미지 데이터

    // height map 정보 읽기 //
    // height map 파일 열기
    std::ifstream FileIn(HeightMap, std::ifstream::in | std::ifstream::binary);
    assert(FileIn.is_open());

    // BITMAP 파일 헤더 읽기
    FileIn.read(reinterpret_cast<char*>(&bitmapFileHeader), sizeof(bitmapFileHeader));
    assert(!FileIn.fail());

    // BITMAP 정보 헤더 읽기
    FileIn.read(reinterpret_cast<char*>(&bitmapInfoHeader), sizeof(bitmapInfoHeader));
    assert(!FileIn.fail());

    // 지형의 크기 저장
    m_TerrainWidth = bitmapInfoHeader.biWidth;
    m_TerrainHeight = bitmapInfoHeader.biHeight;

    // 지형의 정점의 수 계산
    vertexCount = m_TerrainWidth * m_TerrainHeight;

    // 정점 배열 생성
    m_Vertices = std::make_unique<Graphic::Model::PTN_VertexType[]>(vertexCount);
    assert(m_Vertices);

    // BITMAP 이미지 데이터의 크기 가져오기
    imageSize = bitmapInfoHeader.biSizeImage;


    // BITMAP 이미지 데이터 가져오기 //
    // BITMAP 이미지 데이터를 저장할 메모리 생성
    bitmapImage = std::make_unique<unsigned char[]>(imageSize);
    assert(bitmapImage);

    // BITMAP 데이터의 시작 부분으로 이동
    FileIn.seekg(bitmapFileHeader.bfOffBits, std::ios_base::beg);

    // BITMAP 이미지 데이터 읽기
    FileIn.read(reinterpret_cast<char*>(bitmapImage.get()), imageSize);
    assert(!FileIn.fail());

    // 파일을 닫기
    FileIn.close();

    // 이미지 데이터를 높이 맵으로 읽어들입니다.
    k = 0;
    idx = 0;
    for (int i = 0; i < m_TerrainHeight; i++)
    {
        for (int j = 0; j < m_TerrainWidth; j++)
        {
            // 지형의 vectex 배열의 index 계산
            idx = (m_TerrainWidth * i) + j;

            // BITMAP 이미지의 한 pixel(RGB 값)에서 R값만 사용
            height = bitmapImage[k];

            // vertex 데이터의 위치(position) 설정
            m_Vertices[idx].position = DirectX::XMFLOAT4(j, height, i, 1.f);

            // BITMAP 이미지 데이터 배열의 index 증가
            k += 3;
        }
    }

    return true;
}

void Graphic::Terrain::QuadTreeClass::NormalizeHeightMap()
{
    int idx = 0;

    for (int i = 0; i < m_TerrainHeight; i++)
    {
        for (int j = 0; j < m_TerrainWidth; j++)
        {
            idx = (m_TerrainWidth * j) + i;
            m_Vertices[idx].position.y /= 15.f;
        }
    }
}

bool Graphic::Terrain::QuadTreeClass::CalculateNormals()
{
    using namespace DirectX;

    std::unique_ptr<DirectX::XMVECTOR[]> normals;   // normal vector들을 저장할 배열
    std::unique_ptr<int[]> cnt;                     // 지형의 vertex의 normal vector가 총 몇번 더해졌는지 저장하는 배열
    int VertexIndex[4] = { 0, };                    // vertex 배열의 index를 저장할 배열
    DirectX::XMVECTOR vertex[4] = {};               // vertex 데이터의 position 배열
    DirectX::XMVECTOR u = {};                       // u vector
    DirectX::XMVECTOR v = {};                       // v vector
    DirectX::XMVECTOR normal = {};                  // normal vector
    int idx = 0;                                    // vectex 배열의 index
    int size = 0;

    // normal vector를 저장할 배열 생성
    size = m_TerrainHeight * m_TerrainWidth;
    normals = std::make_unique<DirectX::XMVECTOR[]>(size);
    assert(normals);
    cnt = std::make_unique<int[]>(size);
    assert(cnt);

    // 각 vertex의 normal vector 계산
    for (int i = 0; i < (m_TerrainHeight - 1); i++)
    {
        for (int j = 0; j < (m_TerrainWidth - 1); j++)
        {
            // 4개의 꼭지점 가져오기
            VertexIndex[0] = (i * m_TerrainWidth) + j;
            VertexIndex[1] = (i * m_TerrainWidth) + (j + 1);
            VertexIndex[2] = ((i + 1) * m_TerrainWidth) + j;
            VertexIndex[3] = ((i + 1) * m_TerrainWidth) + (j + 1);

            vertex[0] = DirectX::XMLoadFloat4(&m_Vertices[VertexIndex[0]].position);
            vertex[1] = DirectX::XMLoadFloat4(&m_Vertices[VertexIndex[1]].position);
            vertex[2] = DirectX::XMLoadFloat4(&m_Vertices[VertexIndex[2]].position);
            vertex[3] = DirectX::XMLoadFloat4(&m_Vertices[VertexIndex[3]].position);

            // left bottom traingle의 normal vector 계산
            {
                // 표면의 두 vector 계산
                u = DirectX::XMVectorSubtract(vertex[1], vertex[0]);
                v = DirectX::XMVectorSubtract(vertex[2], vertex[0]);

                // 두 vector의 외적 계산
                normal = DirectX::XMVector3Cross(u, v);

                // 각 정점에 normal vector 추가
                normals[VertexIndex[0]] = DirectX::XMVectorAdd(normals[VertexIndex[0]], normal);;
                ++cnt[VertexIndex[0]];

                normals[VertexIndex[1]] = DirectX::XMVectorAdd(normals[VertexIndex[1]], normal);;
                ++cnt[VertexIndex[1]];

                normals[VertexIndex[2]] = DirectX::XMVectorAdd(normals[VertexIndex[2]], normal);;
                ++cnt[VertexIndex[2]];
            }

            // right top traingle의 normal vector 계산
            {
                // 표면의 두 vector 계산
                u = DirectX::XMVectorSubtract(vertex[1], vertex[3]);
                v = DirectX::XMVectorSubtract(vertex[2], vertex[3]);

                // 두 vector의 외적 계산
                normal = DirectX::XMVector3Cross(u, v);

                // 각 정점에 normal vector 추가
                normals[VertexIndex[3]] = DirectX::XMVectorAdd(normals[VertexIndex[3]], normal);;
                ++cnt[VertexIndex[3]];

                normals[VertexIndex[2]] = DirectX::XMVectorAdd(normals[VertexIndex[2]], normal);;
                ++cnt[VertexIndex[2]];

                normals[VertexIndex[1]] = DirectX::XMVectorAdd(normals[VertexIndex[1]], normal);;
                ++cnt[VertexIndex[1]];
            }
        }
    }

    // normal vector의 평균값 구하기
    for (int i = 0; i < m_TerrainHeight; i++)
    {
        for (int j = 0; j < m_TerrainWidth; j++)
        {
            // vertex의 index 가져오기
            idx = (i * m_TerrainWidth) + j;

            // normal vector의 평균값 구하기
            if (cnt[idx])
            {
                normal = normals[idx] / static_cast<float>(cnt[idx]);
                DirectX::XMStoreFloat3(&m_Vertices[idx].normal, normal);
            }
        }
    }

    return true;
}

bool Graphic::Terrain::QuadTreeClass::CreateVertexData(ID3D11Device* device, const char* HeightMap)
{
    // 지형의 높이 맵 로드
    if (!LoadHeightMap(HeightMap))
    {
        return false;
    }

    // height map 표준화
    NormalizeHeightMap();

    // 지형의 normal vector 계산
    if (!CalculateNormals())
    {
        return false;
    }

    // texture 좌표 계산
    CalculateTextureCoordinates();

    return true;
}

void Graphic::Terrain::QuadTreeClass::CreateQuadTree(ID3D11Device* Device, int sy, int sx, int current_height, int current_width, QuadTree* cur)
{
    // 현재 node에 존재하는 삼각형의 개수 계산
    int traingleCount = current_width * current_height * 2;

    // 현재 node의 정보 설정
    cur->height = current_height;
    cur->width = current_width;
    cur->sy = sy;
    cur->sx = sx;

    // QuadTree 생성
    if (traingleCount > MAX_TRAINGLES)
    {
        assert(cur->nodes[0]);
        cur->nodes[0] = std::make_unique<QuadTree>();
        assert(cur->nodes[0]);
        CreateQuadTree(
            Device,
            sy + current_height / 2 - 1,
            sx,
            current_height / 2 + 1,
            current_width / 2,
            cur->nodes[0].get());

        assert(cur->nodes[1]);
        cur->nodes[1] = std::make_unique<QuadTree>();
        assert(cur->nodes[1]);
        CreateQuadTree(
            Device,
            sy + current_height / 2 - 1,
            sx + current_width / 2 - 1,
            current_height / 2 + 1,
            current_width / 2 + 1,
            cur->nodes[1].get());

        assert(cur->nodes[2]);
        cur->nodes[2] = std::make_unique<QuadTree>();
        assert(cur->nodes[2]);
        CreateQuadTree(
            Device,
            sy,
            sx,
            current_height / 2,
            current_width / 2,
            cur->nodes[2].get());

        assert(cur->nodes[3]);
        cur->nodes[3] = std::make_unique<QuadTree>();
        assert(cur->nodes[3]);
        CreateQuadTree(
            Device,
            sy,
            sx + current_width / 2 - 1,
            current_height / 2,
            current_width / 2 + 1,
            cur->nodes[3].get());

        // 현재 node의 최대, 최소점 찾기
        cur->max_x = cur->nodes[0]->max_x;
        cur->max_y = cur->nodes[0]->max_y;
        cur->max_z = cur->nodes[0]->max_z;

        cur->min_x = cur->nodes[0]->min_x;
        cur->min_y = cur->nodes[0]->min_y;
        cur->min_z = cur->nodes[0]->min_z;

        for (int i = 0; i < 4; ++i)
        {
            if (cur->max_x < cur->nodes[i]->max_x) cur->max_x = cur->nodes[i]->max_x;
            if (cur->max_y < cur->nodes[i]->max_y) cur->max_y = cur->nodes[i]->max_y;
            if (cur->max_z < cur->nodes[i]->max_z) cur->max_z = cur->nodes[i]->max_z;

            if (cur->min_x > cur->nodes[i]->min_x) cur->min_x = cur->nodes[i]->min_x;
            if (cur->min_y > cur->nodes[i]->min_y) cur->min_y = cur->nodes[i]->min_y;
            if (cur->min_z > cur->nodes[i]->min_z) cur->min_z = cur->nodes[i]->min_z;
        }
    }
    // vertex buffer, index buffer 생성
    else
        InitializeBuffers(Device, cur);
}

bool Graphic::Terrain::QuadTreeClass::InitializeBuffers(ID3D11Device* Device, QuadTree* node)
{
    D3D11_BUFFER_DESC vertexBufferDesc = {};            // vertex buffer의 정보
    D3D11_SUBRESOURCE_DATA vertexData = {};             // vertex 데이터가 있는 포인터
    D3D11_BUFFER_DESC indexBufferDesc = {};             // index buffer의 정보
    D3D11_SUBRESOURCE_DATA indexData = {};              // index 데이터가 있는 포인터
    int index = 0, pivot = 0;
    HRESULT result = S_OK;

    // 현재 node의 width, height, 시작점 가져오기 //
    int height = node->height;
    int width = node->width;
    int y = node->sy;
    int x = node->sx;

    // 현재 node의 vertex의 개수, index의 수 계산 //
    int vertexCount = height * width;
    int IndexCount = (height - 1) * (width - 1) * 6;

    // vertex 배열, index 배열 생성 //
    std::unique_ptr<Model::PTN_VertexType[]> vertices(std::make_unique<Model::PTN_VertexType[]>(vertexCount));
    assert(vertices);

    std::unique_ptr<ULONG[]> indices(std::make_unique<ULONG[]>(IndexCount));
    assert(indices);

    // index의 개수 저장 //
    node->indexCount = IndexCount;

    // vertex 배열 load 및 현재 node의 최대, 최소점을 찾아서 저장 //
    pivot = (x * m_TerrainWidth) + y;

    node->max_x = m_Vertices[pivot].position.x;
    node->max_y = m_Vertices[pivot].position.y;
    node->max_z = m_Vertices[pivot].position.z;

    node->min_x = m_Vertices[pivot].position.x;
    node->min_y = m_Vertices[pivot].position.y;
    node->min_z = m_Vertices[pivot].position.z;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 현재 node의 vertex 배열의 index 계산
            index = (i * width) + j;

            // 전체 vertex 배열의 index 계산
            pivot = ((y + i) * m_TerrainWidth) + (x + j);

            // vertex 데이터 저장
            vertices[index] = m_Vertices[pivot];

            // 현재 node의 최대, 최소점 찾기
            if (vertices[index].position.x > node->max_x) node->max_x = vertices[index].position.x;
            if (vertices[index].position.y > node->max_y) node->max_y = vertices[index].position.y;
            if (vertices[index].position.z > node->max_z) node->max_z = vertices[index].position.z;

            if (vertices[index].position.x < node->min_x) node->min_x = vertices[index].position.x;
            if (vertices[index].position.y < node->min_y) node->min_y = vertices[index].position.y;
            if (vertices[index].position.z < node->min_z) node->min_z = vertices[index].position.z;
        }
    }

    // index 배열 로드 //
    index = 0;
    pivot = 0;
    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            // 현재 node의 vertex 배열의 index 계산
            pivot = (i * width) + j;

            // left bottom traingle
            indices[index] = pivot;
            ++index;

            indices[index] = pivot + 1;
            ++index;

            indices[index] = pivot + width;
            ++index;

            // right top traingle
            indices[index] = pivot + 1;
            ++index;

            indices[index] = pivot + (width + 1);
            ++index;

            indices[index] = pivot + width;
            ++index;
        }
    }

    // vertex buffer 생성 //
    // vertex buffer 설정
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Model::PTN_VertexType) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // vertex 데이터가 있는 포인터 설정
    vertexData.pSysMem = vertices.get();
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // vertex buffer 생성
    result = Device->CreateBuffer(&vertexBufferDesc, &vertexData, (node->vertexBuffer).GetAddressOf());
    assert(SUCCEEDED(result));


    // index buffer 생성 //
    // index buffer 설정
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(ULONG) * IndexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // index 데이터가 있는 포인터 설정
    indexData.pSysMem = indices.get();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // index buffer 생성
    result = Device->CreateBuffer(&indexBufferDesc, &indexData, (node->indexBuffer).GetAddressOf());
    assert(SUCCEEDED(result));

    return true;
}

void Graphic::Terrain::QuadTreeClass::CalculateTextureCoordinates()
{
    float tu = 0.f;
    float tv = 1.f;
    float incrementU = 0.f;
    float incrementV = 0.f;
    int idx = 0;

    // texture의 width, height 가져오기 //
    int textureWidth = m_Texture->GetTextureInfo().Width;
    int textureHeight = m_Texture->GetTextureInfo().Height;

    // 지형에 texture를 얼마나 반복할지 계산하기 //
    int TEXTURE_REPEAT_U = static_cast<int>(m_TerrainWidth / textureWidth);
    int TEXTURE_REPEAT_V = static_cast<int>(m_TerrainHeight / textureHeight);

    if (!TEXTURE_REPEAT_U)
        TEXTURE_REPEAT_U = 1;
    if (!TEXTURE_REPEAT_V)
        TEXTURE_REPEAT_V = 1;

    // texture 좌표를 얼마나 많이 증가 시킬지 계산 //
    incrementU = static_cast<float>(TEXTURE_REPEAT_U) / static_cast<float>(m_TerrainWidth);
    incrementV = static_cast<float>(TEXTURE_REPEAT_V) / static_cast<float>(m_TerrainHeight);

    // 각 vertex의 texture 좌표 계산 //
    for (int i = 0; i < m_TerrainHeight; i++)
    {
        for (int j = 0; j < m_TerrainWidth; j++)
        {
            // vertex의 index 가져오기
            idx = (m_TerrainWidth * i) + j;
            m_Vertices[idx].texture.x = tu;
            m_Vertices[idx].texture.y = tv;

            // tu 값 증가
            tu += incrementU;

            // tu가 오른쪽 끝에 도달했는지 검사
            if (tu >= 1.f)
                tu = 0.f;
        }

        // tv 값 감소
        tv -= incrementV;

        // tv가 위쪽 끝에 도달했는지 검사
        if (tv <= 0.f)
            tv = 1.f;
    }
}