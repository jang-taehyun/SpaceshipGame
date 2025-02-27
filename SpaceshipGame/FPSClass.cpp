#include "pch.h"
#include "FPSClass.h"

FPSClass::FPSClass() {}
FPSClass::FPSClass(const FPSClass& other) {}
FPSClass::~FPSClass() {}

void FPSClass::Initialize()
{
	// 시작 시간를 현재 시간으로 지정
	m_StartTime = timeGetTime();
}

void FPSClass::Frame()
{
	m_Count++;

	// 1초가 지나면 m_FPS 변수에 현재 frame 값을 저장
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

