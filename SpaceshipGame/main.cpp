#include "pch.h"
#include <iostream>
#include "SystemClass.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	try
	{
		SystemClass system;
		OutputDebugString(_T("SystemClass created success!"));
		system.Run();
	}
	catch (ErrorContent e)
	{
		OutputDebugString(e.title.c_str());
		OutputDebugString(e.contents.c_str());
		OutputDebugString(std::to_wstring(e.errorCode).c_str());
	}

	

	return 0;
}