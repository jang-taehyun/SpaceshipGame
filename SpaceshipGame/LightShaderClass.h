#pragma once

#include "ShaderClass.h"

class LightShaderClass : public ShaderClass
{
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
	bool Render(ID3D11DeviceContext* DeviceContext, int IndexCount, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture, DirectX::XMFLOAT3 LightDirection, DirectX::XMFLOAT4 AmbientColor, DirectX::XMFLOAT4 DiffuseColor, DirectX::XMFLOAT3 CameraPosition, DirectX::XMFLOAT4 SpecularColor, float SpecularPower);

private:
	bool InitializeShader(ID3D11Device* Device, HWND hwnd, const ShaderFileInfo& const info);
	void ShutdownShader();
	bool SetShaderParameters(ID3D11DeviceContext* DeviceContext, DirectX::XMMATRIX WorldMatrix, DirectX::XMMATRIX ViewMatrix, DirectX::XMMATRIX ProjectionMatrix, ID3D11ShaderResourceView* Texture, DirectX::XMFLOAT3 LightDirection, DirectX::XMFLOAT4 AmbientColor, DirectX::XMFLOAT4 DiffuseColor, DirectX::XMFLOAT3 CameraPosition, DirectX::XMFLOAT4 SpecularColor, float SpecularPower);

private:
	ID3D11Buffer* m_LightBuffer = nullptr;
	ID3D11Buffer* m_CameraBuffer = nullptr;
};

