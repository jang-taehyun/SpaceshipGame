#include "pch.h"
#include <time.h>
#include "ModelListClass.h"

ModelListClass::ModelListClass() {}
ModelListClass::ModelListClass(const ModelListClass& other) {}
ModelListClass::~ModelListClass() {}

bool ModelListClass::Initialize(int numModels)
{
	float r = 0.f, g = 0.f, b = 0.f;

	// 3D object의 개수 및 3D object의 정보들을 담은 list 생성 //
	m_ModelCount = numModels;
	m_ModelInfoList = new ModelInfoType[m_ModelCount];
	if (!m_ModelInfoList)
	{
		return false;
	}

	// 랜덤 제네레이터의 seed를 현재 시간으로 하고, random 함수를 통해 3D object의 색상 지정 //
	srand((unsigned int)time(NULL));

	for (int i = 0; i < m_ModelCount; i++)
	{
		// 3D object의 색상을 랜덤하게 지정
		r = (float)(rand() / RAND_MAX);
		g = (float)(rand() / RAND_MAX);
		b = (float)(rand() / RAND_MAX);
		m_ModelInfoList[i].Color = DirectX::XMFLOAT4(r, g, b, 1.f);

		// 3D object의 위치를 랜덤하게 지정
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
