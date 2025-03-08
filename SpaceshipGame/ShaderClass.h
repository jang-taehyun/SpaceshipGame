#pragma once

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
	HRESULT Initialize(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info);

	// shader ��ü ���� �Լ� //
	void Shutdown();

	// shader ������ �Լ� //
	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures);

protected:
	// shader ������ �� ���� �Լ�
	virtual HRESULT InitializeShader(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info);

	// input layout ���� �Լ�
	virtual HRESULT CreateInputLayout(ID3D11Device* const& Device,ID3D10Blob* const& VertexShaderBuffer,ID3D10Blob* const& PixelShaderBuffer);

	// Constant buffer ���� �Լ�
	HRESULT CreateConstantBuffer(ID3D11Device* const& Device, ID3D11Buffer*& Buffer, const UINT& BufferSize);

	// texture sampler state ���� �Լ�
	HRESULT CreateTextureSamplerState(ID3D11Device* const& Device, ID3D11SamplerState*& sampler);

	// shader ���� ���ҽ� ���� �Լ� //
	virtual void ShutdownShader();

	// shader ����� �Լ� //
	void OutputShaderErrorMessage(ID3D10Blob*& ErrorMessage, const HWND& hwnd, const tstring& ShaderFileName);

	// shader ���ο� �� �����͵� ������Ʈ�ϴ� �Լ� //
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures);
	
	// Matrix buffer�� ������Ʈ �ϴ� �Լ� //
	HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix);

	// shader�� ������ �ϴ� �Լ� //
	void RenderShader(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount);

	inline ID3D11InputLayout* const& GetInputLayout() { return m_Layout; }
	inline ID3D11InputLayout** GetInputLayoutPointer() { return &m_Layout; }

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;
};

