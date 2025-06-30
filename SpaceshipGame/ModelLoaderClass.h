#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

#include "IModelLoaderClass.h"

namespace Graphic
{
	namespace Loader
	{
		template<typename VertexType>
		class ModelLoaderClass : public IModelLoaderClass
		{
		public:
			explicit ModelLoaderClass(Model::ID ModelID);
			virtual ~ModelLoaderClass() = default;

			virtual HRESULT Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) override;

			inline ULONG GetMeshCount() const { return m_MeshCount; }
			inline std::vector<std::vector<VertexType>> MoveVerticesDatas() { return std::move(m_Vertices); }
			inline std::vector<std::vector<ULONG>> MoveIndicesDatas() { return std::move(m_Indices); }
			inline std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> MoveMaterialsDatas() { return std::move(m_Materials); }

		private:
			HRESULT LoadVertex();

			virtual std::vector<VertexType> LoadVertexData(aiMesh* mesh) = 0;
			virtual HRESULT LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) = 0;

		protected:
			inline const aiScene* GetScene() const { return m_Scene; }

			inline void PushVerticesData(std::vector<VertexType> vertices) { m_Vertice.push_back(std::move(vertices)); }
			inline void PushIndicesData(std::vector<ULONG> indices) { m_Indices.push_back(std::move(indices)); }
			inline void PushMaterialsData(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> materials) { m_Materials.push_back(std::move(materials)); }

			HRESULT LoadTextureData(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& MaterialList, const aiString& TexturePath);

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