#pragma once

#define MAX(x, y) ((x) > (y) ? (x) : (y))

class SkyDomeClass
{
private:
    struct ModelType
    {
        float x, y, z;
        float tu, tv;
        float nx, ny, nz;
    };

    struct VertexType
    {
        DirectX::XMFLOAT4 position;
    };

public:
    SkyDomeClass() = default;
    ~SkyDomeClass() = default;

    bool Initialize(ID3D11Device*);
    void Render(ID3D11DeviceContext*);

    inline int GetIndexCount() const { return m_indexCount; }
    inline DirectX::XMFLOAT4 GetApexColor() const { return m_apexColor; }
    inline DirectX::XMFLOAT4 GetCenterColor() const { return m_centerColor; }

private:
    bool LoadSkyDomeModel(const char*);

    bool InitializeBuffers(ID3D11Device*);
    void RenderBuffers(ID3D11DeviceContext*);

private:
    std::unique_ptr<ModelType[]> m_model = nullptr;
    int m_vertexCount = 0;
    int m_indexCount = 0;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer = nullptr;
    DirectX::XMFLOAT4 m_apexColor = DirectX::XMFLOAT4(0.0f, 0.15f, 0.66f, 1.0f);
    DirectX::XMFLOAT4 m_centerColor = DirectX::XMFLOAT4(0.81f, 0.38f, 0.66f, 1.0f);
};
