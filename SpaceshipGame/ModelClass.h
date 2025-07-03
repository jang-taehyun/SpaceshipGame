#pragma once

#include "IModelClass.h"

namespace Graphic
{
	namespace Loader
	{
		template<typename VertexType>
		class ModelLoaderClass;
	}
}

namespace Graphic
{
	namespace Model
	{
		template<typename VertexType>
		class ModelClass : public IModelClass
		{
		private:
			const UINT MAX_INSTANCE_COUNT = 5;

		public:
			ModelClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, ID ModelID, Shader::ID ShaderID, Loader::ModelLoaderClass<VertexType>* loader);
			virtual ~ModelClass() = default;

			virtual inline void ResetWorldMatrix() override { m_WorldMatrix.clear(); }
			virtual inline void AddWorldMatrix(DirectX::XMFLOAT4X4& world) override { m_WorldMatrix.push_back(world); }

			virtual void UpdateInstanceBuffer(ID3D11DeviceContext* DeviceContext);
			virtual void RenderMesh(ID3D11DeviceContext* DeviceContext, int MeshIdx) override;

			virtual inline Shader::ID GetShaderID() const override { return m_ShaderID; }
			virtual inline ULONG GetIndexCount(int idx) const override { assert(idx < m_MeshCount); return m_MeshesIndexCount[idx]; }
			virtual inline ULONG GetInstanceCount() const override { assert(m_WorldMatrix.size() < MAX_INSTANCE_COUNT); return m_WorldMatrix.size(); }
			virtual inline const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& GetMaterial(int idx) const override { assert(idx < m_MeshCount); return m_Materials[idx]; }

			virtual inline ULONG GetVertexCount(int idx) const override { assert(idx < m_MeshCount); return m_MeshesVertexCount[idx]; }

		private:
			HRESULT Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::ModelLoaderClass<VertexType>* loader);
			HRESULT InitializeBuffers(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Loader::ModelLoaderClass<VertexType>* loader);
			void InitializeMaterials(Loader::ModelLoaderClass<VertexType>* loader);

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

			std::vector<DirectX::XMFLOAT4X4> m_WorldMatrix;											// 각 object의 world matrix 모음

		public:
			ModelClass(const ModelClass& other) = delete;
			ModelClass(ModelClass&& other) noexcept = delete;
			ModelClass& operator=(const ModelClass& other) = delete;
			ModelClass& operator=(ModelClass&& other) noexcept = delete;
		};
	}
}