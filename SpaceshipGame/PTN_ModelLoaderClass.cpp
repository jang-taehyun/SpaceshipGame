#include "pch.h"
#include "ModelLoaderClass.hpp"
#include "TypeConverterClass.h"
#include "PTN_ModelLoaderClass.h"

Graphic::Loader::PTN_ModelLoaderClass::PTN_ModelLoaderClass(Model::ID ModelID) : ModelLoaderClass<Model::PTN_VertexType>(ModelID) {}

std::vector<Graphic::Model::PTN_VertexType> Graphic::Loader::PTN_ModelLoaderClass::LoadVertexData(aiMesh* mesh)
{
	Model::PTN_VertexType vertex = {};
	std::vector<Model::PTN_VertexType> vertices;					// mesh의 vertex 데이터들

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		vertex.position = DirectX::XMFLOAT4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f);
		vertex.texture = DirectX::XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		vertices.push_back(vertex);

		// OBB 박스를 만들때 사용할 position 데이터 파싱
		PushPositionData(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(vertex.position));
	}

	return vertices;
}

void Graphic::Loader::PTN_ModelLoaderClass::LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene)
{

}