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
	// 생성자, 소멸자 //
	ShaderClass();
	ShaderClass(const ShaderClass& other);
	~ShaderClass();

	// shader 객체 초기화 함수 //
	bool Initialize(ID3D11Device* const Device, const HWND hwnd, const ShaderFileInfo& const info);

	// shader 객체 해제 함수 //
	void Shutdown();

	// shader 렌더링 함수 //
	bool Render(ID3D11DeviceContext* const DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, ID3D11ShaderResourceView* const Texture);

protected:
	// 초기화 관련 함수들 //
	// shader 컴파일 및 생성 함수
	bool InitializeShader(ID3D11Device* const Device, const HWND const hwnd, const ShaderFileInfo& const info);

	// input layout 생성 함수
	HRESULT CreateInputLayout(ID3D11Device* const Device,ID3D10Blob* const VertexShaderBuffer,ID3D10Blob* const PixelShaderBuffer);

	// Constant buffer 생성 함수
	HRESULT CreateConstantBuffer(ID3D11Device* const Device, ID3D11Buffer*& Buffer, const UINT& BufferSize);

	// texture sampler state 생성 함수
	HRESULT CreateTextureSamplerState(ID3D11Device* const Device, ID3D11SamplerState*& sampler);

	// shader 관련 리소스 해제 함수 //
	void ShutdownShader();

	// shader 디버깅 함수 //
	void OutputShaderErrorMessage(ID3D10Blob* ErrorMessage, const HWND hwnd, const TCHAR* ShaderFileName);

	// shader 내부에 들어갈 데이터를 설정하는 함수 //
	bool SetShaderParameters(ID3D11DeviceContext* const DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, ID3D11ShaderResourceView* const Texture);
	
	// shader로 렌더링 하는 함수 //
	void RenderShader(ID3D11DeviceContext* const DeviceContext, const int& IndexCount);

protected:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;
};

