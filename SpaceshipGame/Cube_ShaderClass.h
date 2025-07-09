#pragma once

#include "ShaderClass.hpp"

namespace Graphic
{
    namespace Shader
    {
        class Cube_ShaderClass : public ShaderClass<None_ShaderBuffer>
        {
        public:
            explicit Cube_ShaderClass(ID ShaderID);
            virtual ~Cube_ShaderClass() = default;

            // shader 내부에 들어갈 데이터들 업데이트하는 함수 //
            virtual void UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const None_ShaderBuffer& ShaderBufferDatas) override;

        private:
            // shader에서 사용하는 buffer들을 생성하는 함수
            virtual HRESULT CreateBuffers(ID3D11Device* Device) override;

            // shader buffer를 세팅하는 함수 //
            virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext) override;
        };
    }
}

