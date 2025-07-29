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
	IS_FAILED(InitializeSound(data))
	IS_FAILED(InitializeModel(data))
	IS_FAILED(InitializeUITexture(data))
	IS_FAILED(InitializeShader(data))
	IS_FAILED(InitializeFont(data))

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeResolution(nlohmann::json& data)
{
	System::RESOLUTION resol;

	IS_TRUE(data["resolution count"].empty())
	RESOLUTION_COUNT = data["resolution count"];

	IS_TRUE(data["current resolution index"].empty())
	CURRENT_RESOLUTION_INDEX = data["current resolution index"];

	for (UINT i = 0; i < System::RESOLUTION_COUNT; ++i)
	{
		IS_TRUE((data["resolution"][i]["width"].empty() || data["resolution"][i]["height"].empty()))

			resol.WIDTH = data["resolution"][i]["width"];
		resol.HEIGHT = data["resolution"][i]["height"];

		System::RESOLUTIONS.push_back(resol);
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

bool System::GlobalVariableInitializerClass::InitializeSound(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	IS_TRUE(data["sound count"].empty())
	Sound::SoundIDCount = data["sound count"];

	for (UINT i = 0; i < data["sound path"].size(); ++i)
	{
		IS_TRUE(data["sound path"][i].empty())

		path = data["sound path"][i];
		wpath.assign(path.begin(), path.end());

		Sound::SoundFileList.insert(std::make_pair(static_cast<Sound::ID>(i + 1), wpath));
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeModel(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	IS_TRUE(data["model count"].empty())
	Graphic::Model::ModelIDCount = data["model count"];

	for (UINT i = 0; i < data["model path"].size(); ++i)
	{
		IS_TRUE(data["model path"][i]["path"].empty())

		path = data["model path"][i]["path"];
		Graphic::Model::ModelFileList.insert(std::make_pair(static_cast<Graphic::Model::ID>(i + 1), path));

		if (!data["model path"][i]["texture path"].empty())
		{
			path = data["model path"][i]["texture path"];
			wpath.assign(path.begin(), path.end());
			Graphic::Model::ModelTexturePathList.insert(std::make_pair(static_cast<Graphic::Model::ID>(i + 1), wpath));
		}
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeUITexture(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	IS_TRUE(data["UI texture count"].empty())
	Graphic::Texture::UITextureIDCount = data["UI texture count"];

	for (UINT i = 0; i < data["UI texture path"].size(); ++i)
	{
		IS_TRUE(data["UI texture path"][i].empty())
		path = data["UI texture path"][i];
		wpath.assign(path.begin(), path.end());
		Graphic::Texture::UITextureFileList.insert(std::make_pair(static_cast<Graphic::Texture::UITextureID>(i + 1), wpath));
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeShader(nlohmann::json& data)
{
	IS_TRUE(data["shader count"].empty())
	Graphic::Shader::ShaderIDCount = data["shader count"];

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeFont(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	IS_TRUE(data["font count"].empty())
	Graphic::Font::FontIDCount = data["font count"];

	for (UINT i = 0; i < data["font path"].size(); ++i)
	{
		IS_TRUE(data["font path"][i].empty())
		path = data["font path"][i];
		wpath.assign(path.begin(), path.end());
		Graphic::Font::FontFileList.insert(std::make_pair(static_cast<Graphic::Font::ID>(i + 1), wpath));
	}

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
