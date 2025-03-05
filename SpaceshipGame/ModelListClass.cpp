#include "pch.h"
#include <time.h>
#include "ModelListClass.h"

ModelListClass::ModelListClass() {}
ModelListClass::ModelListClass(const ModelListClass& other) {}
ModelListClass::~ModelListClass() {}

bool ModelListClass::Initialize(int numModels)
{
	float r = 0.f, g = 0.f, b = 0.f;

	// 3D object�� ���� �� 3D object�� �������� ���� list ���� //
	m_ModelCount = numModels;
	m_ModelInfoList = new ModelInfoType[m_ModelCount];
	if (!m_ModelInfoList)
	{
		return false;
	}

	// ���� ���׷������� seed�� ���� �ð����� �ϰ�, random �Լ��� ���� 3D object�� ���� ���� //
	srand((unsigned int)time(NULL));

	for (int i = 0; i < m_ModelCount; i++)
	{
		// 3D object�� ������ �����ϰ� ����
		r = (float)(rand() / RAND_MAX);
		g = (float)(rand() / RAND_MAX);
		b = (float)(rand() / RAND_MAX);
		m_ModelInfoList[i].Color = DirectX::XMFLOAT4(r, g, b, 1.f);

		// 3D object�� ��ġ�� �����ϰ� ����
		m_ModelInfoList[i].PositionX = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].PositionY = (((float)rand() - (float)rand()) / RAND_MAX) * 10.0f;
		m_ModelInfoList[i].PositionZ = ((((float)rand() - (float)rand()) / RAND_MAX) * 10.0f) + 5.0f;
	}
	
	return true;
}

void ModelListClass::Shutdown()
{
	if (m_ModelInfoList)
	{
		delete[] m_ModelInfoList;
		m_ModelInfoList = nullptr;
	}
}

int ModelListClass::GetModelCount()
{
	return m_ModelCount;
}

void ModelListClass::GetData(int index, float& positionX, float& positionY, float& positionZ, DirectX::XMFLOAT4& color)
{
	positionX = m_ModelInfoList[index].PositionX;
	positionY = m_ModelInfoList[index].PositionY;
	positionZ = m_ModelInfoList[index].PositionZ;

	color = m_ModelInfoList[index].Color;
}
