#include "pch.h"
#include "ModelLoaderClass.h"

template<typename VertexType>
Graphic::Loader::ModelLoaderClass<VertexType>::ModelLoaderClass(const std::wstring& filename)
{
	Assimp::Importer importer;				// assimp 라이브러리 importer 객체

	// assimp 라이브러리를 통해 모델 파일을 메모리에 로드 //
	m_Filename.assign(filename.begin(), filename.end());
	m_Scene = importer.ReadFile(m_Filename.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_MakeLeftHanded);
	assert(m_Scene && !m_Scene->HasMeshes());

	// mesh 개수 설정 //
	m_MeshCount = m_Scene->mNumMeshes;
}