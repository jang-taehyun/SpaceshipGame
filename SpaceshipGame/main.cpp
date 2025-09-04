#include "pch.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#endif // _DEBUG

#include "GlobalVariableInitializerClass.h"
#include "SystemClass.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{

#ifdef _DEBUG
	// 메모리 누수 감지 활성화
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(452);
#endif // _DEBUG

	{
		std::unique_ptr<System::GlobalVariableInitializerClass> init = nullptr;
		std::unique_ptr<System::SystemClass> system = nullptr;
		
		init = std::make_unique<System::GlobalVariableInitializerClass>();
		init->InputData();

		system = std::make_unique<System::SystemClass>();
		assert(system);

		system->Run();
		system.reset();

		init->OutputData();
		init.reset();
	}

#ifdef _DEBUG
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif // _DEBUG

	return 0;
}