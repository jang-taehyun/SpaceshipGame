#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "GlobalVariableInitializerClass.h"
#include "SystemClass.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// 메모리 누수 감지 활성화
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(496);

	{
		std::unique_ptr<System::GlobalVariableInitializerClass> init(std::make_unique<System::GlobalVariableInitializerClass>());
		init->InputData();

		std::unique_ptr<System::SystemClass> system(std::make_unique<System::SystemClass>());
		assert(system);

		system->Run();

		Sound::SoundFileList.clear();
		Graphic::Model::ModelFileList.clear();
		Graphic::Model::ModelTexturePathList.clear();
		Graphic::Texture::UITextureFileList.clear();
		Graphic::Font::FontFileList.clear();

		system.reset();
	}

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return 0;
}