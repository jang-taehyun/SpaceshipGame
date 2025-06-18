#include "pch.h"
#include "BackgroundClass.h"

BackgroundClass::BackgroundClass(const ID3D11Device* Device, const ID3D11DeviceContext* DeviceContext, const std::wstring& Filename, DirectX::XMFLOAT2 Position, float Rotation, DirectX::XMFLOAT2 Origin, DirectX::XMFLOAT2 Scale, DirectX::XMFLOAT4 Color) : UIClass(Device, DeviceContext, Filename, Position, Rotation, Origin, Scale, Color) {}

void BackgroundClass::Render() const
{

}