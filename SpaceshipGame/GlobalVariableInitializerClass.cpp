#include "pch.h"
#include <fstream>
#include "GlobalVariableInitializerClass.h"

#define IS_FAILED(x) if(!(x)) return false;
#define IS_TRUE(x) if((x)) return false;

bool System::GlobalVariableInitializerClass::InputData()
{
	std::ifstream FileIn("./data/data.json", std::ios::in);
	nlohmann::json data;

	IS_TRUE((FileIn.fail()));
	FileIn >> data;
	FileIn.close();

	return InitializeGlobalVariable(data);
}

bool System::GlobalVariableInitializerClass::InitializeGlobalVariable(nlohmann::json& data)
{
	IS_FAILED(InitializeResolution(data))
	IS_FAILED(InitializeGraphicSetting(data))
	IS_FAILED(InitializeInputSensitivity(data))

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeResolution(nlohmann::json& data)
{
	IS_TRUE(data["resolution count"].empty())
	RESOLUTION_COUNT = data["resolution count"];

	IS_TRUE(data["current resolution index"].empty())
	CURRENT_RESOLUTION_INDEX = data["current resolution index"];

	for (UINT i = 0; i < System::RESOLUTION_COUNT; ++i)
	{
		IS_TRUE((data["resolution"][i]["width"].empty() || data["resolution"][i]["height"].empty()))

		System::RESOLUTIONS[i].WIDTH = data["resolution"][i]["width"];
		System::RESOLUTIONS[i].HEIGHT = data["resolution"][i]["height"];
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeGraphicSetting(nlohmann::json& data)
{
	IS_TRUE(data["full screen"].empty())
	FULL_SCREEN = (data["full screen"] == "true" ? true : false);

	IS_TRUE(data["VSYNC"].empty())
	VSYNC_ENABLED = (data["VSYNC"] == "true" ? true : false);

	IS_TRUE(data["screen depth"].empty())
	SCREEN_DEPTH = data["screen depth"];

	IS_TRUE(data["screen near"].empty())
	SCREEN_NEAR = data["screen near"];

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeInputSensitivity(nlohmann::json& data)
{
	IS_TRUE(data["mouse sensitivity"].empty())
	MOUSE_SENSITIVITY = data["mouse sensitivity"];

	return true;
}

bool System::GlobalVariableInitializerClass::OutputData()
{
	std::ifstream FileIn("./data/data.json", std::ios::in);
	std::ofstream FileOut;
	nlohmann::json data;

	IS_TRUE(FileIn.fail())
	FileIn >> data;
	FileIn.close();

	IS_FAILED(EditGlobalVariable(data))

	FileOut.open("./data/data.json", std::ios::out);
	IS_TRUE(FileOut.fail())
	FileOut << data;
	FileOut.close();

	return true;
}

bool System::GlobalVariableInitializerClass::EditGlobalVariable(nlohmann::json& data)
{
	IS_FAILED(EditResolution(data))
	IS_FAILED(EditGraphicSetting(data))
	IS_FAILED(EditMouseSensitivity(data))

	return true;
}

bool System::GlobalVariableInitializerClass::EditResolution(nlohmann::json& data)
{
	IS_TRUE(data["current resolution index"].empty())
	data["current resolution index"] = CURRENT_RESOLUTION_INDEX;

	return true;
}

bool System::GlobalVariableInitializerClass::EditGraphicSetting(nlohmann::json& data)
{
	IS_TRUE(data["full screen"].empty())
	data["full screen"] = (FULL_SCREEN ? "true" : "false");

	IS_TRUE(data["VSYNC"].empty())
	data["VSYNC"] = (VSYNC_ENABLED ? "true" : "false");

	return true;
}

bool System::GlobalVariableInitializerClass::EditMouseSensitivity(nlohmann::json& data)
{
	IS_TRUE(data["mouse sensitivity"].empty())
	data["mouse sensitivity"] = MOUSE_SENSITIVITY;

	return true;
}
