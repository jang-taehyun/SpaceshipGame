#include "pch.h"
#include "TimerClass.h"

TimerClass::TimerClass() {}
TimerClass::TimerClass(const TimerClass& other) {}
TimerClass::~TimerClass() {}

bool TimerClass::Initialize()
{
	// 성능 카운터의 빈도 검색
	// high performance timer를 지원하는지 확인
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	if (!m_Frequency)
	{
		return false;
	}

	// 1ms마다 counter에서 tick이 몇 번 일어나는지 계산
	m_TicksPerMs = (float)(m_Frequency / 1000);

	// 성능 카운터의 현재 값(현재 CPU의 tick)을 가져오기
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 CurrentTime = 0;

	// 매 루프마다 시간 차이를 계산해서 frame 간의 시간 간격을 알아내고, m_Frame에 저장
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	float TimeDifference = (float)(CurrentTime - m_StartTime);

	m_FrameTime = TimeDifference / m_TicksPerMs;
	m_StartTime = CurrentTime;
}

float TimerClass::GetTime()
{
	return m_FrameTime;
}
