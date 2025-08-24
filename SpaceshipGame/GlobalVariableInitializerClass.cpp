#include "pch.h"
#include <fstream>
#include "GlobalVariableInitializerClass.h"

void System::GlobalVariableInitializerClass::InputData()
{
	std::ifstream FileIn("./data/data.json");
	nlohmann::json data;

	if (FileIn.is_open())
	{
		FileIn >> data;
		FileIn.close();
		return InitializeGlobalVariable(data);
	}
	else
		return InitializeGlobalVariable();
}

void System::GlobalVariableInitializerClass::InitializeGlobalVariable(nlohmann::json& data)
{
	InitializeResolution(data);
	InitializeGraphicSetting(data);
	InitializeInputSensitivity(data);
}

void System::GlobalVariableInitializerClass::InitializeGlobalVariable()
{
	System::RESOLUTION_COUNT = 2;
	System::CURRENT_RESOLUTION_INDEX = 0;
	System::RESOLUTIONS[0] = { 1280, 800 };
	System::RESOLUTIONS[1] = { 1920, 1080 };

	System::FULL_SCREEN = false;
	System::VSYNC_ENABLED = true;
	System::SCREEN_DEPTH = 1000.f;
	System::SCREEN_NEAR = 0.1f;

	System::MOUSE_SENSITIVITY = 0.0015f;
}

void System::GlobalVariableInitializerClass::InitializeResolution(nlohmann::json& data)
{
	assert(!data["resolution count"].empty());
	RESOLUTION_COUNT = data["resolution count"];

	assert(!data["current resolution index"].empty());
	CURRENT_RESOLUTION_INDEX = data["current resolution index"];

	for (UINT i = 0; i < System::RESOLUTION_COUNT; ++i)
	{
		assert(!(data["resolution"][i]["width"].empty() || data["resolution"][i]["height"].empty()));

		System::RESOLUTIONS[i].WIDTH = data["resolution"][i]["width"];
		System::RESOLUTIONS[i].HEIGHT = data["resolution"][i]["height"];
	}
}

void System::GlobalVariableInitializerClass::InitializeGraphicSetting(nlohmann::json& data)
{
	assert(!data["full screen"].empty());
	FULL_SCREEN = (data["full screen"] == "true" ? true : false);

	assert(!data["VSYNC"].empty());
	VSYNC_ENABLED = (data["VSYNC"] == "true" ? true : false);

	assert(!data["screen depth"].empty());
	SCREEN_DEPTH = data["screen depth"];

	assert(!data["screen near"].empty());
	SCREEN_NEAR = data["screen near"];
}

void System::GlobalVariableInitializerClass::InitializeInputSensitivity(nlohmann::json& data)
{
	assert(!data["mouse sensitivity"].empty());
	MOUSE_SENSITIVITY = data["mouse sensitivity"];
}

void System::GlobalVariableInitializerClass::OutputData()
{
	std::ofstream FileOut;
	nlohmann::json data;

	EditGlobalVariable(data);

	FileOut.open("./data/data.json");
	assert(FileOut.is_open());
	FileOut << data;
	FileOut.close();
}

void System::GlobalVariableInitializerClass::EditGlobalVariable(nlohmann::json& data)
{
	EditResolution(data);
	EditGraphicSetting(data);
	EditMouseSensitivity(data);
}

void System::GlobalVariableInitializerClass::EditResolution(nlohmann::json& data)
{
	data["resolution count"] = RESOLUTION_COUNT;
	data["current resolution index"] = CURRENT_RESOLUTION_INDEX;

	for (UINT i = 0; i < System::RESOLUTION_COUNT; ++i)
	{
		data["resolution"][i]["width"] = System::RESOLUTIONS[i].WIDTH;
		data["resolution"][i]["height"] = System::RESOLUTIONS[i].HEIGHT;
	}
}

void System::GlobalVariableInitializerClass::EditGraphicSetting(nlohmann::json& data)
{
	data["full screen"] = (FULL_SCREEN ? "true" : "false");
	data["VSYNC"] = (VSYNC_ENABLED ? "true" : "false");
	data["screen depth"] = SCREEN_DEPTH;
	data["screen near"] = SCREEN_NEAR;
}

void System::GlobalVariableInitializerClass::EditMouseSensitivity(nlohmann::json& data)
{
	data["mouse sensitivity"] = MOUSE_SENSITIVITY;
}
