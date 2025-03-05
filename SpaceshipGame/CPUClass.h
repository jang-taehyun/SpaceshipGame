#pragma once

#pragma comment(lib, "pdh.lib")
#include <pdh.h>

class CPUClass
{
public:
	CPUClass();
	CPUClass(const CPUClass& other);
	~CPUClass();

	void Intialize();
	void Shutdown();
	void Frame();

	// Getter //
	inline const long long& GetCPUPercentage() const { return (m_CanReadCPU ? m_CPUUsage : 0); }

private:
	bool m_CanReadCPU = true;
	HQUERY m_QueryHandle = 0;
	HCOUNTER m_CounterHandle = 0;
	unsigned long long m_LastSampleTime = 0;
	long long m_CPUUsage = 0;
};