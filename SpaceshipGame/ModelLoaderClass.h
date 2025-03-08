#pragma once

class ModelLoaderClass
{
private:
	// 3D model의 position 관련
	typedef struct _position_type_
	{
		float x, y, z;
	} PositionType;

	// 3D model의 texture coordinate 관련
	typedef struct _texture_coordinate_type_
	{
		float u, v;
	} TexCoordType;

	// 3D model의 normal vector 관련
	typedef struct _normal_vector_type
	{
		float x, y, z;
	} NormalType;

	// 3D model의 face 관련(triangle)
	typedef struct _face_type_triangle_
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	} FaceTypeTraingle;

	// 3D model의 face 관련(polygon)
	typedef struct _face_type_polygon_
	{
		int vIndex1, vIndex2, vIndex3, vIndex4;
		int tIndex1, tIndex2, tIndex3, tIndex4;
		int nIndex1, nIndex2, nIndex3, nIndex4;
	} FaceTypePolygon;

public:
	ModelLoaderClass();
	ModelLoaderClass(const ModelLoaderClass& other);
	~ModelLoaderClass();

	HRESULT LoadModel(const tstring& FileName);
	HRESULT CopyModelData(VertexType* const& storage);
	void ReleaseData();
	
	inline const int GetPositionCount() const { return m_PositionCount; }
	inline const int GetIndexCount() const { return m_IndexCount; }
	inline const int GetFaceIndex() const { return m_FaceIndex; }

private:
	HRESULT ReadModelData(const tstring& FileName);
	HRESULT LoadDataStructuresInTriangle(const tstring& FileName);
	HRESULT LoadDataStructuresInPolygon(const tstring& FileName);
	HRESULT CopyModelDataInTriangle(VertexType* const& storage);
	HRESULT CopyModelDataInPolygon(VertexType* const& storage);

private:
	int m_PositionCount = 0;
	int m_IndexCount = 0;
	int m_TextureCoordCount = 0;
	int m_NormalCount = 0;
	int m_FaceCount = 0;

	int m_FaceIndex = 0;
	bool m_IsTrainge = false;

	PositionType* m_Positions = nullptr;
	TexCoordType* m_TexCoords = nullptr;
	NormalType* m_Normals = nullptr;
	FaceTypeTraingle* m_TraingleFaces = nullptr;
	FaceTypePolygon* m_PolygonFaces = nullptr;

	std::ifstream m_FileIn;
};

