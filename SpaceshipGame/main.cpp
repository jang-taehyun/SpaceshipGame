#include "pch.h"
#include "SystemClass.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	SystemClass* System = new SystemClass;
	if (!System)
		return -1;

	if (S_OK == System->Initialize())
		System->Run();

	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}