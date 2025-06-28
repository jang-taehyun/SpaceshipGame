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
		public:
			ModelClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, ID ModelID);
			virtual ~ModelClass() = default;

			void RenderMesh(ID3D11DeviceContext* DeviceContext, int MeshIdx) override;
			inline ULONG GetIndexCount(int idx) const override { assert(idx < m_MeshCount); return m_MeshesIndexCount[idx]; }
			inline ULONG GetVertexCount(int idx) const { assert(idx < m_MeshCount); return m_MeshesVertexCount[idx]; }
			inline const std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& GetMaterial(int idx) const { assert(idx < m_MeshCount); return m_Materials[idx]; }

		private:
			HRESULT Initialize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) override;
			HRESULT InitializeBuffers(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Graphic::Loader::ModelLoaderClass<VertexType>& loader);
			void InitializeMaterials(Loader::ModelLoaderClass<VertexType>& loader);

		private:
			ID m_ModelID = ID::DEFAULT_SPACESHIP;

			UINT m_MeshCount = 0;
			std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_VertexBuffer;						// vertex buffer
			std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_IndexBuffer;						// index buffer
			std::vector<ULONG> m_MeshesVertexCount;													// 각 mesh에 있는 vertex 데이터의 개수
			std::vector<ULONG> m_MeshesIndexCount;													// 각 mesh에 있는 index 데이터의 개수

			std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> m_Materials;	// 각 mesh에서 사용하는 material 데이터들

		public:
			ModelClass(const ModelClass& other) = delete;
			ModelClass(ModelClass&& other) noexcept = delete;
			ModelClass& operator=(const ModelClass& other) = delete;
			ModelClass& operator=(ModelClass&& other) noexcept = delete;
		};
	}
}