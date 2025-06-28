#include "pch.h"
#include <timeapi.h>
#include "FPSClass.h"

bool System::FPSClass::IsInitailize = false;
static ErrorContent e;

System::FPSClass::FPSClass()
{
	HRESULT result = S_OK;

	// 에러 메세지 초기화 //
	e.title = _T("FPSClass Constructor");

	if (IsInitailize)
	{
		e.contents = _T("이미 FPSClass 인스턴스가 존재합니다.");
		e.errorCode = E_FAIL;
		throw e;
	}

	Initialize();
	IsInitailize = true;
}

System::FPSClass::~FPSClass()
{
	IsInitailize = false;
}

void System::FPSClass::Initialize()
{
	// 시작 시간를 현재 시간으로 지정
	m_StartTime = timeGetTime();
}

void System::FPSClass::Frame()
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

