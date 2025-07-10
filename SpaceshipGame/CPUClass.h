#pragma once

#pragma comment(lib, "pdh.lib")
#include <pdh.h>

namespace System
{
	class CPUClass
	{
	public:
		CPUClass();
		virtual ~CPUClass();

		void Frame();

		// Getter //
		inline ULONGLONG GetCPUPercentage() const { return m_CPUUsage; }

	private:
		void Initialize();
		void Shutdown();

	private:
		static bool IsInitialize;

		bool m_CanReadCPU = false;
		HQUERY m_QueryHandle = 0;
		HCOUNTER m_CounterHandle = 0;
		ULONGLONG m_LastSampleTime = 0;
		ULONGLONG m_CPUUsage = 0;

	public:
		CPUClass(const CPUClass& other) = delete;
		CPUClass(CPUClass&& other) = delete;
		CPUClass& operator=(const CPUClass& other) = delete;
		CPUClass& operator=(CPUClass&& other) = delete;
	};
}