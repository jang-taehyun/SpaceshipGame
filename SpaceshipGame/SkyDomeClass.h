#pragma once

#include <GeometricPrimitive.h>

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
            SkyDomeClass(ID3D11DeviceContext* DeviceContext, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.33f, 0.54f, 0.81f, 1.f));
            virtual ~SkyDomeClass() = default;

            void Render(D3DClass* d3d, DirectX::XMFLOAT4X4 world, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection);

        private:
            void Initialize(ID3D11DeviceContext* DeviceContext);

        private:
            std::unique_ptr<DirectX::GeometricPrimitive> m_SkyDome = nullptr;
            DirectX::XMFLOAT4 m_Color = {};
            std::unique_ptr<Texture::TextureClass> m_Texture = nullptr;
        };
    }
}

