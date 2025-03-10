#include "pch.h"
#include <fstream>
#include "ModelLoaderClass.h"

ModelLoaderClass::ModelLoaderClass() {}
ModelLoaderClass::ModelLoaderClass(const ModelLoaderClass& other) {}
ModelLoaderClass::~ModelLoaderClass() {}

HRESULT ModelLoaderClass::LoadModel(const tstring& FileName)
{
	// file 열기
	m_FileIn.open(FileName);
	if (m_FileIn.fail())
	{
		m_FileIn.close();
		return E_FAIL;
	}

	// File 안의 데이터를 읽어서 data의 개수 구하기
	if (FAILED(ReadModelData(FileName)))
	{
		ReleaseData();
		return E_FAIL;
	}

	// File pointer의 위치를 맨 앞으로 이동
	m_FileIn.clear();
	m_FileIn.seekg(0, std::ios::beg);

	// File 내부의 데이터 읽기 //
	// 기본 polygon이 traingle인 경우
	if (m_IsTrainge)
	{
		// File 안에 있는 vertex, texcoord, normal 데이터 읽기
		if (FAILED(LoadDataStructuresInTriangle(FileName)))
		{
			ReleaseData();
			return E_FAIL;
		}

		m_PositionCount = m_FaceCount * 3;
		m_IndexCount = m_PositionCount;
	}
	// 기본 polygon이 rectangle인 경우
	else
	{
		// File 안에 있는 vertex, texcoord, normal 데이터 읽기
		if (FAILED(LoadDataStructuresInPolygon(FileName)))
		{
			ReleaseData();
			return E_FAIL;
		}

		m_FaceCount *= 2;
		m_PositionCount = m_FaceCount * 3;
		m_IndexCount = m_PositionCount;
	}

	// File 닫기
	m_FileIn.close();

	return S_OK;
}

HRESULT ModelLoaderClass::CopyModelData(VertexType* const& storage)
{
	// 기본 polygon이 traingle인 경우
	if (m_IsTrainge)
	{
		// 읽은 데이터들을 storage에 복사
		if (FAILED(CopyModelDataInTriangle(storage)))
		{
			ReleaseData();
			return E_FAIL;
		}
	}
	// 기본 polygon이 rectangle인 경우
	else
	{
		// 읽은 데이터들을 storage에 복사
		if (FAILED(CopyModelDataInPolygon(storage)))
		{
			ReleaseData();
			return E_FAIL;
		}
	}

	return S_OK;
}

void ModelLoaderClass::ReleaseData()
{
	if (m_FileIn.is_open())
	{
		m_FileIn.close();
	}

	if (m_PolygonFaces)
	{
		delete[] m_PolygonFaces;
		m_PolygonFaces = nullptr;
	}

	if (m_TraingleFaces)
	{
		delete[] m_TraingleFaces;
		m_TraingleFaces = nullptr;
	}

	if (m_Normals)
	{
		delete[] m_Normals;
		m_Normals = nullptr;
	}

	if (m_TexCoords)
	{
		delete[] m_TexCoords;
		m_TexCoords = nullptr;
	}

	if (m_Positions)
	{
		delete[] m_Positions;
		m_Positions = nullptr;
	}
}

HRESULT ModelLoaderClass::ReadModelData(const tstring& FileName)
{
	char input = 0, prev = 0;
	int SlashCount = 0;
	bool IsCount = false;

	// file 안의 데이터 읽기 //
	m_FileIn.get(input);
	while (!m_FileIn.eof())
	{
		if ('v' == input)
		{
			m_FileIn.get(input);

			// 정점의 위치
			if (' ' == input)
				m_PositionCount++;

			// texture 좌표
			if ('t' == input)
				m_TextureCoordCount++;

			// normal vector
			if ('n' == input)
				m_NormalCount++;
		}

		// face
		if ('f' == input)
		{
			m_FileIn.get(input);
			if (' ' == input)
			{
				m_FaceCount++;
				prev = input;
			}
		}


		// 다음 데이터까지 계속 file 안의 데이터 읽기 //
		while ('\n' != input)
		{
			m_FileIn.get(input);

			if ('f' == prev && !IsCount)
			{
				SlashCount++;
			}
		}
		m_FileIn.get(input);

		if ('f' == prev && !IsCount)
		{
			IsCount = true;
			m_IsTrainge = (SlashCount % 8 ? true : false);
		}
	}

	return S_OK;
}

HRESULT ModelLoaderClass::LoadDataStructuresInTriangle(const tstring& FileName)
{
	int VertexIndex = 0, TexCoordIndex = 0, NormalIndex = 0;
	char input, tmp;

	// vertex의 위치, texture 좌표, normal vector, face 데이터 읽어서 왼손 좌표계로 변환해서 메모리에 load //
	m_Positions = new PositionType[m_PositionCount];
	if (!m_Positions)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_TexCoords = new TexCoordType[m_TextureCoordCount];
	if (!m_TexCoords)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_Normals = new NormalType[m_NormalCount];
	if (!m_Normals)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_TraingleFaces = new FaceTypeTraingle[m_FaceCount];
	if (!m_TraingleFaces)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_FileIn.get(input);
	while (!m_FileIn.eof())
	{
		if ('v' == input)
		{
			m_FileIn.get(input);

			// 정점의 position
			if (' ' == input)
			{
				m_FileIn >> m_Positions[VertexIndex].x >> m_Positions[VertexIndex].y >> m_Positions[VertexIndex].z;

				// z 좌표를 뒤집어 왼손 좌표계로 바꾼다.
				m_Positions[VertexIndex].z = m_Positions[VertexIndex].z * -1.f;
				VertexIndex++;
			}

			// texture 좌표
			if ('t' == input)
			{
				m_FileIn >> m_TexCoords[TexCoordIndex].u >> m_TexCoords[TexCoordIndex].v;

				// v 좌표를 왼손 좌표계로 바꾼다.
				m_TexCoords[TexCoordIndex].v = 1.f - m_TexCoords[TexCoordIndex].v;
				TexCoordIndex++;
			}

			// normal vector
			if ('n' == input)
			{
				m_FileIn >> m_Normals[NormalIndex].x >> m_Normals[NormalIndex].y >> m_Normals[NormalIndex].z;

				// z 좌표를 뒤집어 왼손 좌표계로 바꾼다.
				m_Normals[NormalIndex].z = m_Normals[NormalIndex].z * -1.f;
				NormalIndex++;
			}
		}

		// face
		if ('f' == input)
		{
			m_FileIn.get(input);

			// 왼손 좌표계로 바꾸기 위해 face 데이터를 거꾸로 읽기 //
			m_FileIn >> m_TraingleFaces[m_FaceIndex].vIndex[2] >> tmp >> m_TraingleFaces[m_FaceIndex].tIndex[2] >> tmp >> m_TraingleFaces[m_FaceIndex].nIndex[2];
			m_FileIn >> m_TraingleFaces[m_FaceIndex].vIndex[1] >> tmp >> m_TraingleFaces[m_FaceIndex].tIndex[1] >> tmp >> m_TraingleFaces[m_FaceIndex].nIndex[1];
			m_FileIn >> m_TraingleFaces[m_FaceIndex].vIndex[0] >> tmp >> m_TraingleFaces[m_FaceIndex].tIndex[0] >> tmp >> m_TraingleFaces[m_FaceIndex].nIndex[0];

			m_FaceIndex++;
		}

		// 나머지 데이터는 읽고 버리기 //
		while ('\n' != input)
			m_FileIn.get(input);

		// 다음 데이터 읽을 준비 //
		m_FileIn.get(input);
	}

	return S_OK;
}

HRESULT ModelLoaderClass::LoadDataStructuresInPolygon(const tstring& FileName)
{
	int VertexIndex = 0, TexCoordIndex = 0, NormalIndex = 0;
	char input = 0, tmp = 0;

	// vertex의 위치, texture 좌표, normal vector, face 데이터 읽어서 왼손 좌표계로 변환해서 메모리에 load //
	m_Positions = new PositionType[m_PositionCount];
	if (!m_Positions)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_TexCoords = new TexCoordType[m_TextureCoordCount];
	if (!m_TexCoords)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_Normals = new NormalType[m_NormalCount];
	if (!m_Normals)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_PolygonFaces = new FaceTypePolygon[m_FaceCount];
	if (!m_PolygonFaces)
	{
		ReleaseData();
		return E_FAIL;
	}

	m_FileIn.get(input);
	while (!m_FileIn.eof())
	{
		if ('v' == input)
		{
			m_FileIn.get(input);

			// 정점의 position
			if (' ' == input)
			{
				m_FileIn >> m_Positions[VertexIndex].x >> m_Positions[VertexIndex].y >> m_Positions[VertexIndex].z;

				// z 좌표를 뒤집어 왼손 좌표계로 바꾼다.
				m_Positions[VertexIndex].z = m_Positions[VertexIndex].z * -1.f;
				VertexIndex++;
			}

			// texture 좌표
			if ('t' == input)
			{
				m_FileIn >> m_TexCoords[TexCoordIndex].u >> m_TexCoords[TexCoordIndex].v;

				// v 좌표를 왼손 좌표계로 바꾼다.
				m_TexCoords[TexCoordIndex].v = 1.f - m_TexCoords[TexCoordIndex].v;
				TexCoordIndex++;
			}

			// normal vector
			if ('n' == input)
			{
				m_FileIn >> m_Normals[NormalIndex].x >> m_Normals[NormalIndex].y >> m_Normals[NormalIndex].z;

				// z 좌표를 뒤집어 왼손 좌표계로 바꾼다.
				m_Normals[NormalIndex].z = m_Normals[NormalIndex].z * -1.f;
				NormalIndex++;
			}
		}

		// face
		if ('f' == input)
		{
			m_FileIn.get(input);

			// 왼손 좌표계로 바꾸기 위해 face 데이터를 거꾸로 읽기 //
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[3] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[3] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[3];
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[2] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[2] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[2];
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[1] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[1] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[1];
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[0] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[0] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[0];

			m_FaceIndex++;
		}

		// 나머지 데이터는 읽고 버리기 //
		while ('\n' != input)
			m_FileIn.get(input);

		// 다음 데이터 읽을 준비 //
		m_FileIn.get(input);
	}

	return S_OK;
}

HRESULT ModelLoaderClass::CopyModelDataInTriangle(VertexType* const& storage)
{
	int vIndex, tIndex, nIndex;

	// 모든 face를 반복하여 각 면의 3개의 정점을 storage에 복사 //
	for (int i = 0; i < m_FaceIndex; i++)
	{
		for (int j = 0; j < 3; ++j)
		{
			vIndex = m_TraingleFaces[i].vIndex[j] - 1;
			tIndex = m_TraingleFaces[i].tIndex[j] - 1;
			nIndex = m_TraingleFaces[i].nIndex[j] - 1;
			storage[i * 3 + j].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
			storage[i * 3 + j].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
			storage[i * 3 + j].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);
		}
	}

	return S_OK;
}

HRESULT ModelLoaderClass::CopyModelDataInPolygon(VertexType* const& storage)
{
	int vIndex, tIndex, nIndex;

	// 모든 face를 반복하여 각 면의 4개의 정점을 출력 storage에 복사 //
	// 삼각형 팬 방식으로 분할
	// -> 첫 번째 vertex를 기준으로 나머지 정점들을 연결해 삼각형을 생성하는 방식
	for (int i = 0; i < m_FaceIndex; i++)
	{
		// 1, 2, 3번 vertex 데이터 //
		vIndex = m_PolygonFaces[i].vIndex[0] - 1;
		tIndex = m_PolygonFaces[i].tIndex[0] - 1;
		nIndex = m_PolygonFaces[i].nIndex[0] - 1;
		storage[i * 6].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
		storage[i * 6].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
		storage[i * 6].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);

		vIndex = m_PolygonFaces[i].vIndex[1] - 1;
		tIndex = m_PolygonFaces[i].tIndex[1] - 1;
		nIndex = m_PolygonFaces[i].nIndex[1] - 1;
		storage[i * 6 + 1].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
		storage[i * 6 + 1].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
		storage[i * 6 + 1].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);

		vIndex = m_PolygonFaces[i].vIndex[2] - 1;
		tIndex = m_PolygonFaces[i].tIndex[2] - 1;
		nIndex = m_PolygonFaces[i].nIndex[2] - 1;
		storage[i * 6 + 2].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
		storage[i * 6 + 2].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
		storage[i * 6 + 2].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);

		// 1, 3, 4번 vertex 데이터 //
		vIndex = m_PolygonFaces[i].vIndex[0] - 1;
		tIndex = m_PolygonFaces[i].tIndex[0] - 1;
		nIndex = m_PolygonFaces[i].nIndex[0] - 1;
		storage[i * 6 + 3].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
		storage[i * 6 + 3].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
		storage[i * 6 + 3].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);

		vIndex = m_PolygonFaces[i].vIndex[2] - 1;
		tIndex = m_PolygonFaces[i].tIndex[2] - 1;
		nIndex = m_PolygonFaces[i].nIndex[2] - 1;
		storage[i * 6 + 4].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
		storage[i * 6 + 4].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
		storage[i * 6 + 4].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);

		vIndex = m_PolygonFaces[i].vIndex[3] - 1;
		tIndex = m_PolygonFaces[i].tIndex[3] - 1;
		nIndex = m_PolygonFaces[i].nIndex[3] - 1;
		storage[i * 6 + 5].position = DirectX::XMFLOAT3(m_Positions[vIndex].x, m_Positions[vIndex].y, m_Positions[vIndex].z);
		storage[i * 6 + 5].texture = DirectX::XMFLOAT2(m_TexCoords[tIndex].u, m_TexCoords[tIndex].v);
		storage[i * 6 + 5].normal = DirectX::XMFLOAT3(m_Normals[nIndex].x, m_Normals[nIndex].y, m_Normals[nIndex].z);
	}

	return S_OK;
}
