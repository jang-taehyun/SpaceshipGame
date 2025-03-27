#include "pch.h"
#include "FPSClass.h"

bool FPSClass::IsInitailize = false;

FPSClass::FPSClass()
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("FPSClass Constructor");

	if (IsInitailize)
	{
		e.contents = _T("이미 FPSClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;

		throw e;
	}

	result = Initialize();
	if (FAILED(result))
	{
		e.contents = _T("FPSClass 초기화 실패");
		e.errorCode = result;
		throw e;
	}

	IsInitailize = true;
}

FPSClass::~FPSClass()
{
	IsInitailize = false;
}

HRESULT FPSClass::Initialize()
{
	ErrorContent e;
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("FPSClass Initialize()");

	// 시작 시간를 현재 시간으로 지정
	m_StartTime = timeGetTime();

	return result;
}

void FPSClass::Frame()
{
	++m_Count;

	// 1초가 지나면 m_FPS 변수에 현재 frame 값을 저장
	if (timeGetTime() >= (m_StartTime + 1000))
	{
		m_FPS = m_Count;
		m_Count = 0;

		m_StartTime = timeGetTime();
	}
}

