#pragma once

#ifdef _DEBUG
#pragma comment(lib, "assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "assimp-vc143-mt.lib")
#endif

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
			ModelLoaderClass() = default;
			virtual ~ModelLoaderClass() = default;

			virtual void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath) override;

			virtual ULONG GetMeshCount() const override { return m_MeshCount; }
			virtual DirectX::BoundingOrientedBox GetModelOBB() const override { return m_ModelOBB; }
			virtual std::vector<std::vector<ULONG>> MoveIndicesDatas() override { return std::move(m_Indices); }
			virtual std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> MoveMaterialsDatas() override { return std::move(m_Materials); }
			std::vector<std::vector<VertexType>> MoveVerticesDatas() { return std::move(m_Vertices); }

		private:
			void LoadVertex(const aiScene* scene);

			virtual std::vector<VertexType> LoadVertexData(aiMesh* mesh) = 0;
			virtual void LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene, const std::wstring& AdditionalPath) = 0;

		protected:
			void PushPositionData(DirectX::XMFLOAT3 pos) { m_Positions.push_back(pos); }

			void PushVerticesData(std::vector<VertexType> vertices) { m_Vertices.push_back(std::move(vertices)); }
			void PushIndicesData(std::vector<ULONG> indices) { m_Indices.push_back(std::move(indices)); }
			void PushMaterialsData(std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> materials) { m_Materials.push_back(std::move(materials)); }

			HRESULT LoadTextureData(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& MaterialList, const aiString& TexturePath, const std::wstring& AdditionalPath);

		private:
			ULONG m_MeshCount = 0;																				// mesh 개수
			std::vector<std::vector<VertexType>> m_Vertices;													// 각 mesh에 있는 vertex 데이터들
			std::vector<std::vector<ULONG>> m_Indices;															// 각 mesh에 있는 index 데이터들
			std::vector<std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>> m_Materials;				// 각 mesh에서 사용하는 material 데이터들

			std::vector<DirectX::XMFLOAT3> m_Positions;															// 모든 mesh에 존재하는 position 데이터(Bounding oriented box 생성시 사용)
			DirectX::BoundingOrientedBox m_ModelOBB;															// frustum culling용 OBB 박스

		public:
			ModelLoaderClass(const ModelLoaderClass& other) = delete;
			ModelLoaderClass(ModelLoaderClass&& other) noexcept = delete;
			ModelLoaderClass& operator=(const ModelLoaderClass& other) = delete;
			ModelLoaderClass& operator=(ModelLoaderClass&& other) noexcept = delete;
		};
	}
}