#pragma once

#include "ShaderClass.hpp"

namespace Graphic
{
    namespace Shader
    {
        class Cube_ShaderClass : public ShaderClass<M_ShaderBuffer>
        {
        public:
            explicit Cube_ShaderClass(ID ShaderID);
            Cube_ShaderClass(const Cube_ShaderClass& other);
            Cube_ShaderClass(Cube_ShaderClass&& other) noexcept;
            virtual ~Cube_ShaderClass() = default;

            Cube_ShaderClass& operator=(const Cube_ShaderClass& other);
            Cube_ShaderClass& operator=(Cube_ShaderClass&& other) noexcept;

            // shader 내부에 들어갈 데이터들 업데이트하는 함수 //
            virtual void UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const M_ShaderBuffer& ShaderBufferDatas) override;

        private:
            // shader에서 사용하는 buffer들을 생성하는 함수
            virtual HRESULT CreateBuffers(ID3D11Device* Device) override;

            // shader buffer를 세팅하는 함수 //
            virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext) override;

        private:
            // Matrix buffer를 업데이트 하는 함수 //
            HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, const MatrixBufferType& transform);

        private:
            Microsoft::WRL::ComPtr<ID3D11Buffer> m_MatrixBuffer = nullptr;
        };
    }
}

