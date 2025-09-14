#pragma once

#include "ShaderClass.cpp"

namespace Graphic
{
	namespace Shader
	{
		class TerrainShaderClass : public ShaderClass<ML_ShaderBuffers>
		{
			struct WMatrixBufferType
			{
				DirectX::XMMATRIX worldMatrix;						// world
				DirectX::XMMATRIX viewMatrix;						// view
				DirectX::XMMATRIX projectionMatrix;					// projection
			};

		public:
			explicit TerrainShaderClass(ID ShaderID);
			TerrainShaderClass(const TerrainShaderClass& other);
			TerrainShaderClass(TerrainShaderClass&& other) noexcept;
			virtual ~TerrainShaderClass() = default;

			TerrainShaderClass& operator=(const TerrainShaderClass& other);
			TerrainShaderClass& operator=(TerrainShaderClass&& other) noexcept;

			// shader 내부에 들어갈 buffer들을 업데이트하는 함수 //
			virtual void UpdateShaderBuffers(ID3D11DeviceContext* DeviceContext, const ML_ShaderBuffers& ShaderBufferDatas) override;

			void Render(ID3D11DeviceContext* DeviceContext, int IndexCount, int InstanceCount, const std::vector<std::unique_ptr<Texture::TextureClass>>& Material) override;
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
