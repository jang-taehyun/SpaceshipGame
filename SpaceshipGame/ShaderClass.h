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
	ShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~ShaderClass();

	// shader 렌더링 함수 //
	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures);

private:
	// shader 객체 초기화 함수 //
	HRESULT Initialize(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);

	// shader 컴파일 및 생성 함수
	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);

	// input layout 생성 함수
	virtual HRESULT CreateInputLayout(ID3D11Device* const& Device) = 0;

	// texture sampler state 생성 함수
	HRESULT CreateTextureSamplerState(ID3D11Device* const& Device, ID3D11SamplerState*& sampler);

	// shader 객체 해제 함수 //
	void Shutdown();

	// shader 관련 리소스 해제 함수 //
	virtual void ShutdownShader();
	
	// input layout 해제 함수
	virtual void ShutdownLayout() = 0;

	// shader 디버깅 함수 //
	void OutputShaderErrorMessage(ID3D10Blob*& ErrorMessage, const HWND& hwnd, const std::wstring& ShaderFileName);
	
	// Matrix buffer를 업데이트 하는 함수 //
	HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix);

protected:
	// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const std::vector<ID3D11ShaderResourceView*>& Textures);

	// Constant buffer 생성 함수
	HRESULT CreateConstantBuffer(ID3D11Device* const& Device, ID3D11Buffer*& Buffer, const UINT& BufferSize);

	// shader buffer 관련 리소스 해제 함수 //
	void ShutdownShaderBuffer();

	// shader로 렌더링 하는 함수 //
	void RenderShader(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount);

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;

protected:
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D10Blob* m_VertexShaderBuffer = nullptr;
	ID3D10Blob* m_PixelShaderBuffer = nullptr;

public:
	ShaderClass() = delete;
	ShaderClass(const ShaderClass& other) = delete;
};

