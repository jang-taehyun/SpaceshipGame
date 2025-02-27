#include "pch.h"
#include "FPSClass.h"

FPSClass::FPSClass() {}
FPSClass::FPSClass(const FPSClass& other) {}
FPSClass::~FPSClass() {}

void FPSClass::Initialize()
{
	// ���� �ð��� ���� �ð����� ����
	m_StartTime = timeGetTime();
}

void FPSClass::Frame()
{
	m_Count++;

	// 1�ʰ� ������ m_FPS ������ ���� frame ���� ����
	if (timeGetTime() >= (m_StartTime + 1000))
	{
		m_FPS = m_Count;
		m_Count = 0;

		m_StartTime = timeGetTime();
	}
}

int FPSClass::GetFPS()
{
	return m_FPS;
}

