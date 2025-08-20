#include "pch.h"
#include "D3DClass.h"
#include "TextureClass.h"
#include "SkyDomeClass.h"

Graphic::Terrain::SkyDomeClass::SkyDomeClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const WCHAR* texture, DirectX::XMFLOAT4 color) : m_Color(color)
{
	Initialize(Device, DeviceContext, texture);
}

void Graphic::Terrain::SkyDomeClass::Render(D3DClass* d3d, DirectX::XMFLOAT4X4 CameraPos, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection)
{
	DirectX::XMMATRIX w = {}, v = {}, p = {};
	DirectX::XMVECTOR c = {};

	// culling, Z buffer 해제 //
	d3d->TurnOffCulling();
	d3d->TurnDepthBufferOff();

	// sky dome 렌더링 //
	// 각 matrix를 XMMATRIX 형태로 변환
	w = DirectX::XMLoadFloat4x4(&CameraPos);
	v = DirectX::XMLoadFloat4x4(&view);
	p = DirectX::XMLoadFloat4x4(&projection);

	// color를 XMVECTOR 형태로 변환
	c = DirectX::XMLoadFloat4(&m_Color);

	// 렌더링
	m_SkyDome->Draw(w, v, p, c, (m_Texture ? m_Texture->GetTexture() : nullptr));

	// culling, Z buffer 활성화 //
	d3d->TurnDepthBufferOn();
	d3d->TurnOnCulling();
}

void Graphic::Terrain::SkyDomeClass::Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const WCHAR* texture)
{
	m_SkyDome = DirectX::GeometricPrimitive::CreateSphere(DeviceContext, 1000.f);
	assert(m_SkyDome);

	if (texture)
	{
		m_Texture = std::make_unique<Texture::TextureClass>(Device, DeviceContext, texture);
		assert(m_Texture);
	}
}
