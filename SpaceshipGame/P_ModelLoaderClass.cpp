#include "pch.h"
#include "TypeConverterClass.h"
#include "TextureClass.h"
#include "P_ModelLoaderClass.h"

void Graphic::Loader::P_ModelLoaderClass::LoadVertexData(aiMesh* mesh, std::vector<Model::P_VertexType>& vertices)
{
	Model::P_VertexType vertex = {};

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		vertex.position = DirectX::XMFLOAT4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f);

		vertices.push_back(vertex);

		// OBB 박스를 만들때 사용할 position 데이터 파싱
		PushPositionData(Utility::TypeConverterClass::XMFLOAT4toXMFLOAT3(vertex.position));
	}
}

void Graphic::Loader::P_ModelLoaderClass::LoadMaterial(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const aiScene* scene, const std::wstring& AdditionalPath)
{

}