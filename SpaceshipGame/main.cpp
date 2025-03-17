#include "pch.h"
#include "SystemClass.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (S_OK == SystemClass::GetSystemInst()->Initialize())
		SystemClass::GetSystemInst()->Run();

	SystemClass::GetSystemInst()->Shutdown();

	return 0;
}