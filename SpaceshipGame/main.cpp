#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "SystemClass.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// 메모리 누수 감지 활성화
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(393498);

	try
	{
		SystemClass system;
		system.Run();
	}
	catch (ErrorContent e)
	{
		OutputDebugString(e.title.c_str());
		OutputDebugString(_T("\n"));
		OutputDebugString(e.contents.c_str());
		OutputDebugString(_T("\n"));
		OutputDebugString(std::to_wstring(e.errorCode).c_str());
		OutputDebugString(_T("\n"));
	}

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}