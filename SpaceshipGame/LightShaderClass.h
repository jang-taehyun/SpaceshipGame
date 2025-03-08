#pragma once

#include "ShaderClass.h"

class LightShaderClass : public ShaderClass
{
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

private:
	// camera data ����ü //
	struct CameraBufferType
	{
		DirectX::XMFLOAT3 CameraPosition;
		float padding;
	};

	// light data ����ü //
	struct LightBufferType
	{
		DirectX::XMFLOAT4 AmbientColor;
		DirectX::XMFLOAT4 DiffuseColor;
		DirectX::XMFLOAT3 LightDirection;
		float SpecularPower;
		DirectX::XMFLOAT4 SpecularColor;
	};

public:
	HRESULT Render(ID3D11DeviceContext* const& DeviceContext, const int& IndexCount, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT3& CameraPosition, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower);

private:
	HRESULT InitializeShader(ID3D11Device* const& Device, const HWND& hwnd, const ShaderFileInfo& info) override;
	void ShutdownShader() override;
	HRESULT CreateInputLayout(ID3D11Device* const& Device, ID3D10Blob* const& VertexShaderBuffer, ID3D10Blob* const& PixelShaderBuffer) override;
	
	HRESULT SetShaderParameters(ID3D11DeviceContext* const& DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const std::vector<ID3D11ShaderResourceView*>& Textures, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT3& CameraPosition, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower);

	// ���� ��� ������ ������ ������Ʈ�ϴ� �Լ� //
	HRESULT UpdateLightBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower);

	// ī�޶� ��� ������ ������ ������Ʈ�ϴ� �Լ� //
	HRESULT UpdateCameraBuffer(ID3D11DeviceContext* const& DeviceContext, unsigned int& slot, const DirectX::XMFLOAT3& CameraPosition);

private:
	ID3D11Buffer* m_LightBuffer = nullptr;
	ID3D11Buffer* m_CameraBuffer = nullptr;
};

