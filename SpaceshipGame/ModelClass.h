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
			const ULONG MAX_INSTANCE_COUNT = 5;

		public:
			ModelClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, ID ModelID, Shader::ID ShaderID, Loader::IModelLoaderClass* loader);
			ModelClass(const ModelClass& other);
			ModelClass(ModelClass&& other) noexcept;
			virtual ~ModelClass() = default;

			ModelClass<VertexType>& operator=(const ModelClass& other);
			ModelClass<VertexType>& operator=(ModelClass&& other) noexcept;

			virtual inline void AddWorldMatrix(const InstanceBufferType& world) override { m_WorldMatrix.push_back(world); }

			virtual void UpdateInstanceBuffer(ID3D11DeviceContext* DeviceContext) override;
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, UINT MeshIdx) override;

			virtual inline UINT GetMeshCount() const override { return m_MeshCount; }
			virtual inline Shader::ID GetShaderID() const override { return m_ShaderID; }
			virtual inline ULONG GetIndexCount(UINT idx) const override { assert(idx < m_MeshCount); return m_MeshesIndexCount[idx]; }
			virtual inline ULONG GetInstanceCount() const override { assert(static_cast<ULONG>(m_WorldMatrix.size()) < MAX_INSTANCE_COUNT); return static_cast<ULONG>(m_WorldMatrix.size()); }
			virtual inline const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& GetMaterial(UINT idx) const override { assert(idx < m_MeshCount); return m_Materials[idx]; }
			virtual inline DirectX::BoundingOrientedBox GetModelOBB() const { return m_ModelOBB; }

			virtual inline ULONG GetVertexCount(UINT idx) const override { assert(idx < m_MeshCount); return m_MeshesVertexCount[idx]; }

		private:
			HRESULT Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::IModelLoaderClass* loader);
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

			std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> m_Materials;	// 각 mesh에서 사용하는 material 데이터들

			std::vector<InstanceBufferType> m_WorldMatrix;											// 각 object의 world matrix 모음

			DirectX::BoundingOrientedBox m_ModelOBB;
		};
	}
}