#include "pch.h"
#include "TypeConverterClass.h"
#include "TextureClass.h"
#include "PTN_DNSR_ModelLoaderClass.h"

void Graphic::Loader::PTN_DNSR_ModelLoaderClass::LoadVertexData(aiMesh* mesh, std::vector<Model::PTN_VertexType>& vertices)
{
	Model::PTN_VertexType vertex = {};

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		// vertex data 파싱
		vertex.position = DirectX::XMFLOAT4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f);
		vertex.texture = DirectX::XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		vertices.push_back(vertex);

		// OBB 박스를 만들때 사용할 position 데이터 파싱
		PushPositionData(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(vertex.position));
	}
}

void Graphic::Loader::PTN_DNSR_ModelLoaderClass::LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene, const std::wstring& AdditionalPath)
{
	aiMesh* mesh = nullptr;														// scene에 존재하는 mesh 객체
	aiMaterial* material = nullptr;												// mesh에 존재하는 material 객체
	aiString TexturePath;														// texture 경로
	aiReturn ret = aiReturn_SUCCESS;											// assimp 함수의 리턴값
	std::vector<std::unique_ptr<Texture::TextureClass>> MaterialList;			// material 데이터 리스트

	// texture 데이터 파싱 //
	for (ULONG i = 0; i < GetMeshCount(); ++i)
	{
		mesh = scene->mMeshes[i];
		material = scene->mMaterials[mesh->mMaterialIndex];

		// diffuse texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath, AdditionalPath);

		// normal texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_NORMALS, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath, AdditionalPath);

		// specular texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_SPECULAR, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath, AdditionalPath);

		// reflect texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_REFLECTION, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath, AdditionalPath);

		// 현재 mesh의 material 데이터 저장 //
		PushMaterialsData(MaterialList);
	}
}