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
	// 생성자, 소멸자 //
	ShaderClass();
	ShaderClass(const ShaderClass& other);
	~ShaderClass();

	// shader 객체 초기화 함수 //
	HRESULT Initialize(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info);

	// shader 객체 해제 함수 //
	void Shutdown();

	// shader 렌더링 함수 //
	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures);

protected:
	// shader 컴파일 및 생성 함수
	virtual HRESULT InitializeShader(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info);

	// input layout 생성 함수
	virtual HRESULT CreateInputLayout(ID3D11Device* const& Device,ID3D10Blob* const& VertexShaderBuffer,ID3D10Blob* const& PixelShaderBuffer);

	// Constant buffer 생성 함수
	HRESULT CreateConstantBuffer(ID3D11Device* const& Device, ID3D11Buffer*& Buffer, const UINT& BufferSize);

	// texture sampler state 생성 함수
	HRESULT CreateTextureSamplerState(ID3D11Device* const& Device, ID3D11SamplerState*& sampler);

	// shader 관련 리소스 해제 함수 //
	virtual void ShutdownShader();

	// shader 디버깅 함수 //
	void OutputShaderErrorMessage(ID3D10Blob*& ErrorMessage, const HWND& hwnd, const tstring& ShaderFileName);

	// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures);
	
	// Matrix buffer를 업데이트 하는 함수 //
	HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix);

	// shader로 렌더링 하는 함수 //
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

