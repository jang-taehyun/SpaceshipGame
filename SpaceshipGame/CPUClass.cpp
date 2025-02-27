#include "pch.h"
#include "CPUClass.h"

CPUClass::CPUClass() {}
CPUClass::CPUClass(const CPUClass& other) {}
CPUClass::~CPUClass() {}

void CPUClass::Intialize()
{
	PDH_STATUS Status;

	// CPU ��뷮�� polling�� query object ����
	// ���� query object ������ �����ߴٸ�, OS�� ���Ѷ����� ����� ����� �� ����
	Status = PdhOpenQuery(NULL, 0, &m_QueryHandle);
	if (ERROR_SUCCESS != Status)
	{
		m_CanReadCPU = false;
	}

	// �ý����� ��� CPU�� polling�ϵ��� query object�� ����
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
		// query Ƚ���� ���̱� ���� 1�ʸ��� CPU ��뷮�� ���ø�
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
