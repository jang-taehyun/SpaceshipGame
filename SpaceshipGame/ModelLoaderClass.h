#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

namespace Graphic
{
	namespace Loader
	{
		template<typename VertexType>
		class ModelLoaderClass
		{
		public:
			explicit ModelLoaderClass(const std::wstring& filename);
			virtual ~ModelLoaderClass() = default;

			virtual HRESULT Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) = 0;

			inline ULONG GetMeshCount() const { return m_MeshCount; }
			inline std::vector<std::vector<VertexType>>& GetVertices() { return m_Vertices; }
			inline std::vector<std::vector<ULONG>>& GetIndices() { return m_Indices; }
			inline std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>>& GetMaterials() { return m_Materials; }

		protected:
			inline const aiScene* GetScene() const { return m_Scene; }

		private:
			std::string m_Filename = "";
			aiScene* m_Scene = nullptr;																			// model 파일을 로드한 scene 객체

			ULONG m_MeshCount = 0;																				// mesh 개수
			std::vector<std::vector<VertexType>> m_Vertices;													// 각 mesh에 있는 vertex 데이터들
			std::vector<std::vector<ULONG>> m_Indices;															// 각 mesh에 있는 index 데이터들
			std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> m_Materials;				// 각 mesh에서 사용하는 material 데이터들

		public:
			ModelLoaderClass(const ModelLoaderClass& other) = delete;
			ModelLoaderClass(ModelLoaderClass&& other) noexcept = delete;
			ModelLoaderClass& operator=(const ModelLoaderClass& other) = delete;
			ModelLoaderClass& operator=(ModelLoaderClass&& other) noexcept = delete;
		};
	}
}