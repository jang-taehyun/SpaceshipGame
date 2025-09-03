#pragma once

#include "IModelClass.h"

namespace Graphic
{
	namespace Loader { class IModelLoaderClass; }
}

namespace Graphic
{
	namespace Model
	{
		template<typename VertexType>
		class ModelClass : public IModelClass
		{
		private:
			const ULONG MAX_INSTANCE_COUNT = 30;

		public:
			ModelClass(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, ID ModelID, Shader::ID ShaderID, Loader::IModelLoaderClass* loader);
			ModelClass(const ModelClass& other);
			ModelClass(ModelClass&& other) noexcept;
			virtual ~ModelClass() = default;

			ModelClass<VertexType>& operator=(const ModelClass& other);
			ModelClass<VertexType>& operator=(ModelClass&& other) noexcept;

			virtual void AddWorldMatrix(const InstanceBufferType& world) override { m_WorldMatrixAndColor.push_back(world); }

			virtual void UpdateInstanceBuffer(ID3D11DeviceContext* DeviceContext) override;
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, UINT MeshIdx) override;

			virtual UINT GetMeshCount() const override { return m_MeshCount; }
			virtual Shader::ID GetShaderID() const override { return m_ShaderID; }
			virtual ULONG GetIndexCount(UINT idx) const override { assert(idx < m_MeshCount); return m_MeshesIndexCount[idx]; }
			virtual ULONG GetInstanceCount() const override { assert(m_InstanceCount < MAX_INSTANCE_COUNT); return m_InstanceCount; }
			virtual const std::vector<std::unique_ptr<Texture::TextureClass>>& GetMaterial(UINT idx) const override;
			virtual DirectX::BoundingOrientedBox GetModelOBB() const { return m_ModelOBB; }

			virtual ULONG GetVertexCount(UINT idx) const override { assert(idx < m_MeshCount); return m_MeshesVertexCount[idx]; }

		private:
			HRESULT Initialize(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::IModelLoaderClass* loader);
			HRESULT InitializeBuffers(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::IModelLoaderClass* loader);
			void InitializeMaterials(Loader::IModelLoaderClass* loader);

		private:
			ID m_ModelID = ID::NONE;
			Shader::ID m_ShaderID = Shader::ID::NONE;

			UINT m_MeshCount = 0;
			std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_VertexBuffer;						// vertex buffer
			std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_IndexBuffer;						// index buffer
			Microsoft::WRL::ComPtr<ID3D11Buffer> m_InstanceBuffer;									// instance buffer

			std::vector<ULONG> m_MeshesVertexCount;													// 각 mesh에 있는 vertex 데이터의 개수
			std::vector<ULONG> m_MeshesIndexCount;													// 각 mesh에 있는 index 데이터의 개수

			std::vector<std::vector<std::unique_ptr<Texture::TextureClass>>> m_Materials;			// 각 mesh에서 사용하는 material 데이터들
			std::vector<std::unique_ptr<Texture::TextureClass>> m_Empty;							// model에 material이 없는 경우 반환 용도

			UINT m_InstanceCount = 0;
			std::vector<InstanceBufferType> m_WorldMatrixAndColor;									// 각 object의 world matrix, Color 모음

			DirectX::BoundingOrientedBox m_ModelOBB;
		};
	}
}