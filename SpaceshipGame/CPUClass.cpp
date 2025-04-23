#include "pch.h"
#include "CPUClass.h"

bool CPUClass::IsInitialize = false;
static ErrorContent e;

CPUClass::CPUClass()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("CPUClass Constructor");

	if (IsInitialize)
	{
		e.contents = _T("이미 CPUClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	result = Initialize();
	if (FAILED(result))
		Shutdown();

	IsInitialize = true;
}

CPUClass::~CPUClass()
{
	Shutdown();
	IsInitialize = false;
}

HRESULT CPUClass::Initialize()
{
	HRESULT result = S_OK;
	PDH_STATUS Status;

	// 에러 메세지 초기화 //
	e.title = _T("CPUClass Initialize()");

	// CPU 사용량을 polling할 query object 생성 //
	// 만약 query object 생성에 실패했다면, OS의 권한때문에 기능을 사용할 수 없음
	Status = PdhOpenQuery(NULL, 0, &m_QueryHandle);
	if (ERROR_SUCCESS != Status)
	{
		e.contents = _T("CPU 사용량을 polling할 query object 생성 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	// 시스템의 모든 CPU를 polling하도록 query object를 설정 //
	Status = PdhAddCounter(m_QueryHandle, TEXT("\\Process(_Total)\\% processor time"), 0, &m_CounterHandle);
	if (ERROR_SUCCESS != Status)
	{
		e.contents = _T("시스템의 모든 CPU를 polling하도록 query object를 설정 실패");
		e.errorCode = E_FAIL;
		return E_FAIL;
	}

	m_LastSampleTime = GetTickCount64();
	m_CanReadCPU = true;

	return result;
}

void CPUClass::Shutdown()
{
	if (m_CanReadCPU || m_QueryHandle)
	{
		PdhCloseQuery(m_QueryHandle);
		m_QueryHandle = 0;
	}

	m_CanReadCPU = false;
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
