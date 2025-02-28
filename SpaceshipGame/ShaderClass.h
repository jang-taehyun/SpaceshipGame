#pragma once

typedef struct _shader_file_info_
{
	TCHAR* vsFileName = nullptr;
	TCHAR* psFileName = nullptr;
	char* vsEntryPoint = nullptr;
	char* psEntryPoint = nullptr;
} ShaderFileInfo;

class ShaderClass
{
protected:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX World;
		DirectX::XMMATRIX View;
		DirectX::XMMATRIX Projection;
	};

public:
	// ������, �Ҹ��� //
	ShaderClass();
	ShaderClass(const ShaderClass& other);
	~ShaderClass();

	// shader ��ü �ʱ�ȭ �Լ� //
	bool Initialize(ID3D11Device* const Device, const HWND hwnd, const ShaderFileInfo& const info);

	// shader ��ü ���� �Լ� //
	void Shutdown();

	// shader ������ �Լ� //
	bool Render(ID3D11DeviceContext* const DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, ID3D11ShaderResourceView* const Texture);

protected:
	// �ʱ�ȭ ���� �Լ��� //
	// shader ������ �� ���� �Լ�
	bool InitializeShader(ID3D11Device* const Device, const HWND const hwnd, const ShaderFileInfo& const info);

	// input layout ���� �Լ�
	HRESULT CreateInputLayout(ID3D11Device* const Device,ID3D10Blob* const VertexShaderBuffer,ID3D10Blob* const PixelShaderBuffer);

	// Constant buffer ���� �Լ�
	HRESULT CreateConstantBuffer(ID3D11Device* const Device, ID3D11Buffer*& Buffer, const UINT& BufferSize);

	// texture sampler state ���� �Լ�
	HRESULT CreateTextureSamplerState(ID3D11Device* const Device, ID3D11SamplerState*& sampler);

	// shader ���� ���ҽ� ���� �Լ� //
	void ShutdownShader();

	// shader ����� �Լ� //
	void OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, const HWND hwnd, const TCHAR* ShaderFileName);

	// shader ���ο� �� �����͸� �����ϴ� �Լ� //
	bool SetShaderParameters(ID3D11DeviceContext* const DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, ID3D11ShaderResourceView* const Texture);
	
	// shader�� ������ �ϴ� �Լ� //
	void RenderShader(ID3D11DeviceContext* const DeviceContext, const int& IndexCount);

protected:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;
};

