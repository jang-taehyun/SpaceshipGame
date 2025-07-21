#include "pch.h"
#include "ModelLoaderClass.hpp"
#include "TypeConverterClass.h"
#include "PTN_ModelLoaderClass.h"

Graphic::Loader::PTN_ModelLoaderClass::PTN_ModelLoaderClass(Model::ID ModelID) : ModelLoaderClass<Model::PTN_VertexType>(ModelID) {}

std::vector<Graphic::Model::PTN_VertexType> Graphic::Loader::PTN_ModelLoaderClass::LoadVertexData(aiMesh* mesh)
{
	Model::PTN_VertexType vertex = {};
	std::vector<Model::PTN_VertexType> vertices;					// mesh의 vertex 데이터들

	for (UINT j = 0; j < mesh->mNumVertices; ++j)
	{
		vertex.position = DirectX::XMFLOAT4(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.f);
		vertex.texture = DirectX::XMFLOAT2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
		vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

		vertices.push_back(vertex);

		// OBB 박스를 만들때 사용할 position 데이터 파싱
		PushPositionData(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(vertex.position));
	}

	return vertices;
}

void Graphic::Loader::PTN_ModelLoaderClass::LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene)
{

}