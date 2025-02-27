#include "pch.h"
#include "CPUClass.h"

CPUClass::CPUClass() {}
CPUClass::CPUClass(const CPUClass& other) {}
CPUClass::~CPUClass() {}

void CPUClass::Intialize()
{
	PDH_STATUS Status;

	// CPU 사용량을 polling할 query object 생성
	// 만약 query object 생성에 실패했다면, OS의 권한때문에 기능을 사용할 수 없음
	Status = PdhOpenQuery(NULL, 0, &m_QueryHandle);
	if (ERROR_SUCCESS != Status)
	{
		m_CanReadCPU = false;
	}

	// 시스템의 모든 CPU를 polling하도록 query object를 설정
	Status = PdhAddCounter(m_QueryHandle, TEXT("\\Process(_Total)\\% processor time"), 0, &m_CounterHandle);
	if (ERROR_SUCCESS != Status)
	{
		m_CanReadCPU = false;
	}

	m_LastSampleTime = GetTickCount64();
}

void CPUClass::Shutdown()
{
	if (m_CanReadCPU)
	{
		PdhCloseQuery(m_QueryHandle);
	}
}

void CPUClass::Frame()
{
	PDH_FMT_COUNTERVALUE Value;

	if (m_CanReadCPU)
	{
		// query 횟수를 줄이기 위해 1초마다 CPU 사용량을 샘플링
		if (GetTickCount64() > (m_LastSampleTime + (unsigned long long)1000))
		{
			m_LastSampleTime = GetTickCount64();

			PdhCollectQueryData(m_QueryHandle);
			PdhGetFormattedCounterValue(m_CounterHandle, PDH_FMT_LARGE, NULL, &Value);
			m_CPUUsage = Value.largeValue;
		}
	}
}

long long CPUClass::GetCPUPercentage()
{
	return (m_CanReadCPU ? m_CPUUsage : 0);
}
