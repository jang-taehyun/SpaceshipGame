#pragma once
#include "UIClass.h"

class BackgroundClass : public UIClass
{
public:
	BackgroundClass(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext, const std::wstring& Filename, DirectX::XMFLOAT2 Position = DirectX::XMFLOAT2(0.f, 0.f), float Rotation = 0.f, DirectX::XMFLOAT2 Origin = DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT2 Scale = DirectX::XMFLOAT2(0.f, 0.f), DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f));
	virtual ~BackgroundClass() = default;

	virtual void Render() const override;
};

