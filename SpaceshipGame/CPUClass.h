#pragma once

#pragma comment(lib, "pdh.lib")
#include <pdh.h>

class CPUClass
{
public:
	CPUClass();
	~CPUClass();
	
	void Frame();

	// Getter //
	inline const long long& GetCPUPercentage() const { return m_CPUUsage; }

private:
	HRESULT Initialize();
	void Shutdown();

private:
	static bool IsInitialize;

	bool m_CanReadCPU = true;
	HQUERY m_QueryHandle = 0;
	HCOUNTER m_CounterHandle = 0;
	unsigned long long m_LastSampleTime = 0;
	long long m_CPUUsage = 0;

public:
	CPUClass(const CPUClass& other) = delete;
};