#pragma once

#include "ShaderClass.hpp"

namespace Graphic
{
	namespace Shader
	{
		class TerrainShaderClass : public ShaderClass<ML_ShaderBuffers>
		{
		public:
			explicit TerrainShaderClass(ID ShaderID);
			TerrainShaderClass(const TerrainShaderClass& other);
			TerrainShaderClass(TerrainShaderClass&& other) noexcept;
			virtual ~TerrainShaderClass() = default;

			TerrainShaderClass& operator=(const TerrainShaderClass& other);
			TerrainShaderClass& operator=(TerrainShaderClass&& other) noexcept;

			// shader 내부에 들어갈 데이터들 업데이트하는 함수 //
			virtual void UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const ML_ShaderBuffers& ShaderBufferDatas) override;

			void Render(ID3D11DeviceContext* DeviceContext, int IndexCount, ID3D11ShaderResourceView* texture);

		private:
			// shader에서 사용하는 buffer들을 생성하는 함수
			virtual HRESULT CreateBuffers(ID3D11Device* Device) override;

			// shader buffer를 세팅하는 함수 //
			virtual void SetShaderBuffers(ID3D11DeviceContext* DeviceContext) override;

		private:
			// Matrix buffer를 업데이트 하는 함수 //
			HRESULT UpdateMatrixBuffer(ID3D11DeviceContext* DeviceContext, const MatrixBufferType& transform);

			// Light buffer를 업데이트 하는 함수 //
			HRESULT UpdateLightBuffer(ID3D11DeviceContext* DeviceContext, const LightBufferType& light);

		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_MatrixBuffer = nullptr;
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_LightBuffer = nullptr;
		};
	}
}
