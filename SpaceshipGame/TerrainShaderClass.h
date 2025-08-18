#pragma once

class TerrainShaderClass
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

	struct LightBufferType
	{
		DirectX::XMFLOAT4 AmbientColor;
		DirectX::XMFLOAT4 DiffuseColor;
		DirectX::XMFLOAT3 LightDirection;
		float padding = 0.f;
	};

public:
	TerrainShaderClass() = default;
	~TerrainShaderClass() = default;

	bool Initialize(ID3D11Device*, HWND);
	bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMFLOAT3, DirectX::XMFLOAT4, DirectX::XMFLOAT4, ID3D11ShaderResourceView*);
	
	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMFLOAT3, DirectX::XMFLOAT4, DirectX::XMFLOAT4, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Layout = nullptr;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SampleState = nullptr;

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_MatrixBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightBuffer = nullptr;
};
