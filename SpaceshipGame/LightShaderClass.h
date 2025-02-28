#pragma once

#include "ShaderClass.h"

class LightShaderClass : public ShaderClass
{
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

private:
	// camera data 구조체 //
	struct CameraBufferType
	{
		DirectX::XMFLOAT3 CameraPosition;
		float padding;
	};

	// light data 구조체 //
	struct LightBufferType
	{
		DirectX::XMFLOAT4 AmbientColor;
		DirectX::XMFLOAT4 DiffuseColor;
		DirectX::XMFLOAT3 LightDirection;
		float SpecularPower;
		DirectX::XMFLOAT4 SpecularColor;
	};

public:
	HRESULT Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, const int& TextureNum, ID3D11ShaderResourceView** const TextureArray, DirectX::XMFLOAT3 LightDirection, DirectX::XMFLOAT4 AmbientColor, DirectX::XMFLOAT4 DiffuseColor, DirectX::XMFLOAT3 CameraPosition, DirectX::XMFLOAT4 SpecularColor, float SpecularPower);

private:
	HRESULT InitializeShader(ID3D11Device* const Device, const HWND const hwnd, const ShaderFileInfo& const info) override;
	void ShutdownShader() override;
	HRESULT CreateInputLayout(ID3D11Device* const Device, ID3D10Blob* const VertexShaderBuffer, ID3D10Blob* const PixelShaderBuffer) override;
	
	HRESULT SetShaderParameters(ID3D11DeviceContext* const DeviceContext, const DirectX::XMMATRIX& WorldMatrix, const DirectX::XMMATRIX& ViewMatrix, const DirectX::XMMATRIX& ProjectionMatrix, const int& TextureNum, ID3D11ShaderResourceView** const TextureArray, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT3& CameraPosition, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower);

	// 광원 상수 버퍼의 내용을 업데이트하는 함수 //
	HRESULT UpdateLightBuffer(ID3D11DeviceContext* const DeviceContext, unsigned int slot, const DirectX::XMFLOAT3& LightDirection, const DirectX::XMFLOAT4& AmbientColor, const DirectX::XMFLOAT4& DiffuseColor, const DirectX::XMFLOAT4& SpecularColor, const float& SpecularPower);

	// 카메라 상수 버퍼의 내용을 업데이트하는 함수 //
	HRESULT UpdateCameraBuffer(ID3D11DeviceContext* const DeviceContext, unsigned int slot, const DirectX::XMFLOAT3& CameraPosition);

private:
	ID3D11Buffer* m_LightBuffer = nullptr;
	ID3D11Buffer* m_CameraBuffer = nullptr;
};

