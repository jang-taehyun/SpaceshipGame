#include "pch.h"
#include "TextureClass.h"
#include "PTN_DNSR_ModelLoaderClass.h"

Graphic::Loader::PTN_ModelLoaderClass::PTN_ModelLoaderClass(const std::wstring& filename) : ModelLoaderClass<PTN_VertexType>(filename) {}

HRESULT Graphic::Loader::PTN_ModelLoaderClass::Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	HRESULT result = S_OK;

	// vertex 데이터 로드 //
	result = LoadVertexData();
	if (FAILED(result))
		return result;

	// material 데이터 로드 //
	result = LoadMaterialData(Device, DeviceContext);
	if (FAILED(result))
		return result;

	return result;
}

HRESULT Graphic::Loader::PTN_ModelLoaderClass::LoadVertexData()
{
	HRESULT result = S_OK;
	aiMesh* mesh = nullptr;					// scene에 존재하는 mesh 객체

	std::vector<PTN_VertexType> vertices;	// mesh의 vertex 데이터들
	aiFace face;							// mesh의 face
	std::vector<ULONG> indices;				// mesh의 index 데이터들
	
	PTN_VertexType vertex = {};				// vertex 데이터
	
	// 메쉬 개수를 가져와서 가져온 메쉬 개수만큼 데이터(vertex, index, texture) 파싱 //
	for (ULONG i = 0; i < GetMeshCount(); ++i)
	{
		mesh = GetScene()->mMeshes[i];
		
		// vertex 데이터 파싱 //
		for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
		{
			vertex.position = DirectX::XMFLOAT3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z);
			vertex.texture = DirectX::XMFLOAT2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
			vertex.normal = DirectX::XMFLOAT3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z);

			vertices.push_back(vertex);
		}
		GetVertices().push_back(std::move(vertices));

		// index 데이터 파싱 //
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
		{
			face = mesh->mFaces[j];

			for(unsigned int k = 0; k < face.mNumIndices; ++k)
				indices.push_back(face.mIndices[k]);
		}
		GetIndices().push_back(std::move(indices));
	}

	return result;
}

HRESULT Graphic::Loader::PTN_ModelLoaderClass::LoadMaterialData(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
{
	HRESULT result = S_OK;
	aiMesh* mesh = nullptr;														// scene에 존재하는 mesh 객체
	aiMaterial* material = nullptr;												// mesh에 존재하는 material 객체
	aiString TexturePath;														// texture 경로
	aiReturn ret = aiReturn_SUCCESS;											// assimp 함수의 리턴값
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> MaterialList;	// material 데이터 리스트

	// texture 데이터 파싱 //
	for (ULONG i = 0; i < GetMeshCount(); ++i)
	{
		mesh = GetScene()->mMeshes[i];
		material = GetScene()->mMaterials[mesh->mMaterialIndex];

		// diffuse texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_DIFFUSE, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTexture(Device, DeviceContext, MaterialList, TexturePath);

		// normal texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_NORMALS, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTexture(Device, DeviceContext, MaterialList, TexturePath);

		// specular texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_SPECULAR, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTexture(Device, DeviceContext, MaterialList, TexturePath);

		// reflect texture 파싱 및 저장 //
		ret = material->GetTexture(aiTextureType_REFLECTION, 0, &TexturePath);
		if (ret == aiReturn_SUCCESS)
			LoadTexture(Device, DeviceContext, MaterialList, TexturePath);

		// 현재 mesh의 material 데이터 저장 //
		GetMaterials().push_back(std::move(MaterialList));
	}

	return result;
}

HRESULT Graphic::Loader::PTN_ModelLoaderClass::LoadTexture(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>& MaterialList, const aiString& TexturePath)
{
	HRESULT result = S_OK;
	std::string path;
	std::wstring wpath;
	std::unique_ptr<Texture::TextureClass> texture = nullptr;		// texture 객체

	// texture 경로를 std::wstring으로 변환
	path = TexturePath.C_Str();
	wpath.assign(path.begin(), path.end());

	// texture 생성 및 저장
	texture = std::make_unique<Texture::TextureClass>(Device, DeviceContext, wpath);
	MaterialList.push_back(std::move(texture->GetTexture()));

	return result;
}
