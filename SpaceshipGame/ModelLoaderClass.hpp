#pragma once

#include "pch.h"
#include "TextureClass.h"
#include "ModelLoaderClass.h"

template<typename VertexType>
void Graphic::Loader::ModelLoaderClass<VertexType>::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath)
{
	Assimp::Importer importer;				// assimp 라이브러리 importer 객체

	// assimp 라이브러리를 통해 모델 파일을 메모리에 로드 //
	const aiScene* scene = importer.ReadFile(ModelFilename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_MakeLeftHanded);
	assert(scene);
	assert(scene->HasMeshes());

	// mesh 개수 설정 //
	m_MeshCount = scene->mNumMeshes;

	// vertex 데이터 로드 //
	LoadVertex(scene);

	// material 데이터 로드 //
	LoadMaterial(Device, DeviceContext, scene, AdditionalPath);

	// Model의 OBB 박스 생성(frustum culling용 OBB 박스) //
	DirectX::BoundingOrientedBox::CreateFromPoints(m_ModelOBB, m_Positions.size(), m_Positions.data(), sizeof(DirectX::XMFLOAT3));

	// importer 객체 해제
	importer.FreeScene();
}

template<typename VertexType>
void Graphic::Loader::ModelLoaderClass<VertexType>::LoadVertex(const aiScene* scene)
{
	aiMesh* mesh = nullptr;					// scene에 존재하는 mesh 객체

	std::vector<VertexType> vertices;		// mesh의 vertex 데이터들
	aiFace face;							// mesh의 face
	std::vector<ULONG> indices;				// mesh의 index 데이터들

	VertexType vertex = {};					// vertex 데이터

	// 메쉬 개수를 가져와서 가져온 메쉬 개수만큼 데이터(vertex, index, texture) 파싱 //
	for (ULONG i = 0; i < GetMeshCount(); ++i)
	{
		mesh = scene->mMeshes[i];

		// vertex 데이터 파싱 //
		PushVerticesData(std::move(LoadVertexData(mesh)));

		// index 데이터 파싱 //
		for (UINT j = 0; j < mesh->mNumFaces; ++j)
		{
			face = mesh->mFaces[j];

			for (UINT k = 0; k < face.mNumIndices; ++k)
				indices.push_back(face.mIndices[k]);
		}
		PushIndicesData(indices);
	}
}

template<typename VertexType>
HRESULT Graphic::Loader::ModelLoaderClass<VertexType>::LoadTextureData(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& MaterialList, const aiString& TexturePath, const std::wstring& AdditionalPath)
{
	HRESULT result = S_OK;
	std::string path;
	std::wstring wpath;
	std::unique_ptr<Texture::TextureClass> texture = nullptr;		// texture 객체

	// texture 경로를 std::wstring으로 변환
	path = TexturePath.C_Str();
	wpath = wpath.assign(path.begin(), path.end());

	// 프로젝트의 상대 경로로 변경
	wpath = AdditionalPath + wpath;

	// texture 생성 및 저장
	texture = std::make_unique<Texture::TextureClass>(Device, DeviceContext, wpath);
	MaterialList.push_back(texture->MoveTexture());

	return result;
}