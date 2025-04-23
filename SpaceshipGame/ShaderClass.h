#pragma once

class LightClass;
class CameraClass;

class ShaderClass
{
protected:
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
		DirectX::XMFLOAT4 SpecularColor;
		float SpecularPower;
	};

	struct CameraBufferType
	{
		DirectX::XMFLOAT3 CameraPosition;
		float padding;
	};

public:
	// 생성자, 소멸자 //
	explicit ShaderClass(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);
	virtual ~ShaderClass();

	// shader 렌더링 함수 //
	virtual HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const std::vector<ID3D11ShaderResourceView*>& Textures);

private:
	// shader 객체 초기화 함수 //
	HRESULT Initialize(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);

	// shader 컴파일 및 생성 함수
	virtual HRESULT InitializeShader(const HWND& hwnd, ID3D11Device* const& Device, const ShaderFileInfo& info);

	// input layout 생성 함수
	HRESULT CreateInputLayout(ID3D11Device* const& Device, ID3D10Blob* const& VertexShaderBuffer);

	// texture sampler state 생성 함수
	HRESULT CreateTextureSamplerState(ID3D11Device* const& Device, ID3D11SamplerState*& sampler);

	// shader 객체 해제 함수 //
	void Shutdown();

	// shader 관련 리소스 해제 함수 //
	void ShutdownShader();

	// shader 디버깅 함수 //
	void OutputShaderErrorMessage(ID3D10Blob*& ErrorMessage, const HWND& hwnd, const std::wstring& ShaderFileName);

	void ShutdownShaderBufferErrorMessage(ID3D10Blob*& VertexShaderBuffer, ID3D10Blob*& PixelShaderBuffer, ID3D10Blob*& ErrorMessage);

protected:
	// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
	virtual HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const TransformMatrixData& transform, const LightClass* const& light, const CameraClass* const& camera, const std::vector<ID3D11ShaderResourceView*>& Textures);

	// Constant buffer 생성 함수
	HRESULT CreateConstantBuffer(ID3D11Device* const& Device, ID3D11Buffer*& Buffer, const UINT& BufferSize);

	// shader로 렌더링 하는 함수 //
	void RenderShader(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount);

	// Matrix buffer를 업데이트 하는 함수 //
	HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const TransformMatrixData& transform);

	// Light buffer를 업데이트 하는 함수 //
	HRESULT UpdateLightBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const LightClass* const& light);

	// camera buffer를 업데이트 하는 함수 //
	HRESULT UpdateCameraBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const CameraClass* const& camera);

private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11Buffer* m_LightBuffer = nullptr;
	ID3D11Buffer* m_CameraBuffer = nullptr;
	ID3D11SamplerState* m_SampleState = nullptr;

public:
	ShaderClass() = delete;
	ShaderClass(const ShaderClass& other) = delete;
};

