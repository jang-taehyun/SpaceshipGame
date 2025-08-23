#pragma once

#include <GeometricPrimitive.h>
#include <Effects.h>

namespace Graphic
{
    class D3DClass;

    namespace Texture { class TextureClass; }
}

namespace Graphic
{
    namespace Terrain
    {
        class SkyDomeClass
        {
        public:
            SkyDomeClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const WCHAR* texture, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.33f, 0.54f, 0.81f, 1.f));
            virtual ~SkyDomeClass() = default;

            void Render(D3DClass* d3d, DirectX::XMFLOAT4X4 CameraPos, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);

        private:
            void Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const WCHAR* texture);

        private:
            std::unique_ptr<DirectX::GeometricPrimitive> m_SkyDome = nullptr;
            std::unique_ptr<DirectX::BasicEffect> m_Effect = nullptr;
            Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Layout = nullptr;
            DirectX::XMFLOAT4 m_Color = {};
            std::unique_ptr<Texture::TextureClass> m_Texture = nullptr;
        };
    }
}

