#include "pch.h"
#include <fstream>
#include "ModelLoaderClass.h"

ModelLoaderClass::ModelLoaderClass() {}
ModelLoaderClass::ModelLoaderClass(const ModelLoaderClass& other) {}
ModelLoaderClass::~ModelLoaderClass() {}

HRESULT ModelLoaderClass::LoadModel(const tstring& FileName)
{
	// file ����
	m_FileIn.open(FileName);
	if (m_FileIn.fail())
	{
		m_FileIn.close();
		return E_FAIL;
	}

	// File ���� �����͸� �о data�� ���� ���ϱ�
	if (FAILED(ReadModelData(FileName)))
	{
		ReleaseData();
		return E_FAIL;
	}

	// File pointer�� ��ġ�� �� ������ �̵�
	m_FileIn.clear();
	m_FileIn.seekg(0, std::ios::beg);

	// File ������ ������ �б� //
	// �⺻ polygon�� traingle�� ���
	if (m_IsTrainge)
	{
		// File �ȿ� �ִ� vertex, texcoord, normal ������ �б�
		if (FAILED(LoadDataStructuresInTriangle(FileName)))
		{
			ReleaseData();
			return E_FAIL;
		}

		m_PositionCount = m_FaceCount * 3;
		m_IndexCount = m_PositionCount;
	}
	// �⺻ polygon�� rectangle�� ���
	else
	{
		// File �ȿ� �ִ� vertex, texcoord, normal ������ �б�
		if (FAILED(LoadDataStructuresInPolygon(FileName)))
		{
			ReleaseData();
			return E_FAIL;
		}

		m_FaceCount *= 2;
		m_PositionCount = m_FaceCount * 3;
		m_IndexCount = m_PositionCount;
	}

	// File �ݱ�
	m_FileIn.close();

	return S_OK;
}

HRESULT ModelLoaderClass::CopyModelData(VertexType* const& storage)
{
	// �⺻ polygon�� traingle�� ���
	if (m_IsTrainge)
	{
		// ���� �����͵��� storage�� ����
		if (FAILED(CopyModelDataInTriangle(storage)))
		{
			ReleaseData();
			return E_FAIL;
		}
	}
	// �⺻ polygon�� rectangle�� ���
	else
	{
		// ���� �����͵��� storage�� ����
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

	// file ���� ������ �б� //
	m_FileIn.get(input);
	while (!m_FileIn.eof())
	{
		if ('v' == input)
		{
			m_FileIn.get(input);

			// ������ ��ġ
			if (' ' == input)
				m_PositionCount++;

			// texture ��ǥ
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


		// ���� �����ͱ��� ��� file ���� ������ �б� //
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

	// vertex�� ��ġ, texture ��ǥ, normal vector, face ������ �о �޼� ��ǥ��� ��ȯ�ؼ� �޸𸮿� load //
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

			// ������ position
			if (' ' == input)
			{
				m_FileIn >> m_Positions[VertexIndex].x >> m_Positions[VertexIndex].y >> m_Positions[VertexIndex].z;

				// z ��ǥ�� ������ �޼� ��ǥ��� �ٲ۴�.
				m_Positions[VertexIndex].z = m_Positions[VertexIndex].z * -1.f;
				VertexIndex++;
			}

			// texture ��ǥ
			if ('t' == input)
			{
				m_FileIn >> m_TexCoords[TexCoordIndex].u >> m_TexCoords[TexCoordIndex].v;

				// v ��ǥ�� �޼� ��ǥ��� �ٲ۴�.
				m_TexCoords[TexCoordIndex].v = 1.f - m_TexCoords[TexCoordIndex].v;
				TexCoordIndex++;
			}

			// normal vector
			if ('n' == input)
			{
				m_FileIn >> m_Normals[NormalIndex].x >> m_Normals[NormalIndex].y >> m_Normals[NormalIndex].z;

				// z ��ǥ�� ������ �޼� ��ǥ��� �ٲ۴�.
				m_Normals[NormalIndex].z = m_Normals[NormalIndex].z * -1.f;
				NormalIndex++;
			}
		}

		// face
		if ('f' == input)
		{
			m_FileIn.get(input);

			// �޼� ��ǥ��� �ٲٱ� ���� face �����͸� �Ųٷ� �б� //
			m_FileIn >> m_TraingleFaces[m_FaceIndex].vIndex[2] >> tmp >> m_TraingleFaces[m_FaceIndex].tIndex[2] >> tmp >> m_TraingleFaces[m_FaceIndex].nIndex[2];
			m_FileIn >> m_TraingleFaces[m_FaceIndex].vIndex[1] >> tmp >> m_TraingleFaces[m_FaceIndex].tIndex[1] >> tmp >> m_TraingleFaces[m_FaceIndex].nIndex[1];
			m_FileIn >> m_TraingleFaces[m_FaceIndex].vIndex[0] >> tmp >> m_TraingleFaces[m_FaceIndex].tIndex[0] >> tmp >> m_TraingleFaces[m_FaceIndex].nIndex[0];

			m_FaceIndex++;
		}

		// ������ �����ʹ� �а� ������ //
		while ('\n' != input)
			m_FileIn.get(input);

		// ���� ������ ���� �غ� //
		m_FileIn.get(input);
	}

	return S_OK;
}

HRESULT ModelLoaderClass::LoadDataStructuresInPolygon(const tstring& FileName)
{
	int VertexIndex = 0, TexCoordIndex = 0, NormalIndex = 0;
	char input = 0, tmp = 0;

	// vertex�� ��ġ, texture ��ǥ, normal vector, face ������ �о �޼� ��ǥ��� ��ȯ�ؼ� �޸𸮿� load //
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

			// ������ position
			if (' ' == input)
			{
				m_FileIn >> m_Positions[VertexIndex].x >> m_Positions[VertexIndex].y >> m_Positions[VertexIndex].z;

				// z ��ǥ�� ������ �޼� ��ǥ��� �ٲ۴�.
				m_Positions[VertexIndex].z = m_Positions[VertexIndex].z * -1.f;
				VertexIndex++;
			}

			// texture ��ǥ
			if ('t' == input)
			{
				m_FileIn >> m_TexCoords[TexCoordIndex].u >> m_TexCoords[TexCoordIndex].v;

				// v ��ǥ�� �޼� ��ǥ��� �ٲ۴�.
				m_TexCoords[TexCoordIndex].v = 1.f - m_TexCoords[TexCoordIndex].v;
				TexCoordIndex++;
			}

			// normal vector
			if ('n' == input)
			{
				m_FileIn >> m_Normals[NormalIndex].x >> m_Normals[NormalIndex].y >> m_Normals[NormalIndex].z;

				// z ��ǥ�� ������ �޼� ��ǥ��� �ٲ۴�.
				m_Normals[NormalIndex].z = m_Normals[NormalIndex].z * -1.f;
				NormalIndex++;
			}
		}

		// face
		if ('f' == input)
		{
			m_FileIn.get(input);

			// �޼� ��ǥ��� �ٲٱ� ���� face �����͸� �Ųٷ� �б� //
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[3] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[3] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[3];
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[2] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[2] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[2];
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[1] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[1] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[1];
			m_FileIn >> m_PolygonFaces[m_FaceIndex].vIndex[0] >> tmp >> m_PolygonFaces[m_FaceIndex].tIndex[0] >> tmp >> m_PolygonFaces[m_FaceIndex].nIndex[0];

			m_FaceIndex++;
		}

		// ������ �����ʹ� �а� ������ //
		while ('\n' != input)
			m_FileIn.get(input);

		// ���� ������ ���� �غ� //
		m_FileIn.get(input);
	}

	return S_OK;
}

HRESULT ModelLoaderClass::CopyModelDataInTriangle(VertexType* const& storage)
{
	int vIndex, tIndex, nIndex;

	// ��� face�� �ݺ��Ͽ� �� ���� 3���� ������ storage�� ���� //
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

	// ��� face�� �ݺ��Ͽ� �� ���� 4���� ������ ��� storage�� ���� //
	// �ﰢ�� �� ������� ����
	// -> ù ��° vertex�� �������� ������ �������� ������ �ﰢ���� �����ϴ� ���
	for (int i = 0; i < m_FaceIndex; i++)
	{
		// 1, 2, 3�� vertex ������ //
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

		// 1, 3, 4�� vertex ������ //
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
