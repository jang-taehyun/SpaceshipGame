#pragma once

class SkyDomeShaderClass
{
private:
    struct MatrixBufferType
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    struct GradientBufferType
    {
        DirectX::XMFLOAT4 apexColor;
        DirectX::XMFLOAT4 centerColor;
    };

public:
    SkyDomeShaderClass() = default;
    ~SkyDomeShaderClass() = default;

    bool Initialize(ID3D11Device*, HWND);
    bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMFLOAT4, DirectX::XMFLOAT4);

private:
    bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

    bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMFLOAT4, DirectX::XMFLOAT4);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_layout = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_matrixBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_gradientBuffer = nullptr;
};