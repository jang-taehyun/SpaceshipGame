#include "pch.h"
#include "TypeConverterClass.h"
#include "PTN_DNSR_ModelLoaderClass.h"

Graphic::Loader::PTN_DNSR_ModelLoaderClass::PTN_DNSR_ModelLoaderClass(Model::ID ModelID) : ModelLoaderClass<Model::PTN_VertexType>(ModelID) {}

std::vector<Graphic::Model::PTN_VertexType> Graphic::Loader::PTN_DNSR_ModelLoaderClass::LoadVertexData(aiMesh* mesh)
{
	Model::PTN_VertexType vertex = {};
	std::vector<Model::PTN_VertexType> vertices;					// mesh의 vertex 데이터들

	for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
	{
		// vertex data 파싱
		vertex.position = DirectX::XMFLOAT4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.f);
		vertex.texture = DirectX::XMFLOAT2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
		vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

		vertices.push_back(vertex);

		// OBB 박스를 만들때 사용할 position 데이터 파싱
		PushPositionData(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(vertex.position));
	}

	return vertices;
}

void Graphic::Loader::PTN_DNSR_ModelLoaderClass::LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene)
{
	aiMesh* mesh = nullptr;														// scene에 존재하는 mesh 객체
	aiMaterial* material = nullptr;												// mesh에 존재하는 material 객체
	aiString TexturePath;														// texture 경로
	aiReturn ret = aiReturn_SUCCESS;											// assimp 함수의 리턴값
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> MaterialList;	// material 데이터 리스트

	// texture 데이터 파싱 //
	for (ULONG i = 0; i < GetMeshCount(); ++i)
	{
		mesh = scene->mMeshes[i];
		material = scene->mMaterials[mesh->mMaterialIndex];

		// diffuse texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath);

		// normal texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_NORMALS, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath);

		// specular texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_SPECULAR, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath);

		// reflect texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_REFLECTION, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTextureData(Device, DeviceContext, MaterialList, TexturePath);

		// 현재 mesh의 material 데이터 저장 //
		PushMaterialsData(MaterialList);
	}
}