#include "pch.h"
#include "TimerClass.h"

TimerClass::TimerClass() {}
TimerClass::TimerClass(const TimerClass& other) {}
TimerClass::~TimerClass() {}

bool TimerClass::Initialize()
{
	// ���� ī������ �� �˻�
	// high performance timer�� �����ϴ��� Ȯ��
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	if (!m_Frequency)
	{
		return false;
	}

	// 1ms���� counter���� tick�� �� �� �Ͼ���� ���
	m_TicksPerMs = (float)(m_Frequency / 1000);

	// ���� ī������ ���� ��(���� CPU�� tick)�� ��������
	QueryPerformanceCounter((LARGE_INTEGER*)&m_StartTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 CurrentTime = 0;

	// �� �������� �ð� ���̸� ����ؼ� frame ���� �ð� ������ �˾Ƴ���, m_Frame�� ����
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
	float TimeDifference = (float)(CurrentTime - m_StartTime);

	m_FrameTime = TimeDifference / m_TicksPerMs;
	m_StartTime = CurrentTime;
}

float TimerClass::GetTime()
{
	return m_FrameTime;
}
