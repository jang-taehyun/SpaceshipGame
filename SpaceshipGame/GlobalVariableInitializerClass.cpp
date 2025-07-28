#include "pch.h"
#include <fstream>
#include "GlobalVariableInitializerClass.h"

bool System::GlobalVariableInitializerClass::InputData()
{
	std::ifstream FileIn("./data/data.json", std::ios::in);
	nlohmann::json data;

	if (FileIn.fail())
		return false;

	FileIn >> data;

	FileIn.close();

	return InitializeGlobalVariable(data);
}

bool System::GlobalVariableInitializerClass::InitializeGlobalVariable(nlohmann::json& data)
{
	if (!InitializeResolution(data))
		return false;

	if (!InitializeGraphicSetting(data))
		return false;

	if (!InitializeInputSensitivity(data))
		return false;

	if (!InitializeSound(data))
		return false;

	if (!InitializeModel(data))
		return false;

	if (!InitializeUITexture(data))
		return false;

	if (!InitializeShader(data))
		return false;

	if (!InitializeFont(data))
		return false;

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeResolution(nlohmann::json& data)
{
	System::RESOLUTION resol;

	if (data["resolution count"].empty())
		return false;
	RESOLUTION_COUNT = data["resolution count"];

	if (data["current resolution index"].empty())
		return false;
	CURRENT_RESOLUTION_INDEX = data["current resolution index"];

	for (UINT i = 0; i < System::RESOLUTION_COUNT; ++i)
	{
		if (data["resolution"][i]["width"].empty() || data["resolution"][i]["height"].empty())
			return false;

		resol.WIDTH = data["resolution"][i]["width"];
		resol.HEIGHT = data["resolution"][i]["height"];

		System::RESOLUTIONS.push_back(resol);
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeGraphicSetting(nlohmann::json& data)
{
	if (data["full screen"].empty())
		return false;
	FULL_SCREEN = (data["full screen"] == "true" ? true : false);

	if (data["VSYNC"].empty())
		return false;
	VSYNC_ENABLED = (data["VSYNC"] == "true" ? true : false);

	if (data["screen depth"].empty())
		return false;
	SCREEN_DEPTH = data["screen depth"];

	if (data["screen near"].empty())
		return false;
	SCREEN_NEAR = data["screen near"];

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeInputSensitivity(nlohmann::json& data)
{
	if (data["mouse sensitivity"].empty())
		return false;
	MOUSE_SENSITIVITY = data["mouse sensitivity"];

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeSound(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	if (data["sound count"].empty())
		return false;
	Sound::SoundIDCount = data["sound count"];

	for (UINT i = 0; i < data["sound path"].size(); ++i)
	{
		if (data["sound path"][i].empty())
			return false;

		path = data["sound path"][i];
		wpath.assign(path.begin(), path.end());

		Sound::SoundFileList.insert(std::make_pair(static_cast<Sound::ID>(i+1), wpath));
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeModel(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	if (data["model count"].empty())
		return false;
	Graphic::Model::ModelIDCount = data["model count"];

	for (UINT i = 0; i < data["model path"].size(); ++i)
	{
		if (data["model path"][i]["path"].empty())
			return false;

		path = data["model path"][i]["path"];

		Graphic::Model::ModelFileList.insert(std::make_pair(static_cast<Graphic::Model::ID>(i+1), path));

		if (!data["model path"][i]["texture path"].empty())
		{
			path = data["model path"][i]["texture path"];
			wpath.assign(path.begin(), path.end());

			Graphic::Model::ModelTexturePathList.insert(std::make_pair(static_cast<Graphic::Model::ID>(i+1), wpath));
		}
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeUITexture(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	if (data["UI texture count"].empty())
		return false;
	Graphic::Texture::UITextureIDCount = data["UI texture count"];

	for (UINT i = 0; i < data["UI texture path"].size(); ++i)
	{
		if (data["UI texture path"][i].empty())
			return false;

		path = data["UI texture path"][i];
		wpath.assign(path.begin(), path.end());

		Graphic::Texture::UITextureFileList.insert(std::make_pair(static_cast<Graphic::Texture::UITextureID>(i+1), wpath));
	}

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeShader(nlohmann::json& data)
{
	if (data["shader count"].empty())
		return false;
	Graphic::Shader::ShaderIDCount = data["shader count"];

	return true;
}

bool System::GlobalVariableInitializerClass::InitializeFont(nlohmann::json& data)
{
	std::string path;
	std::wstring wpath;

	if (data["font count"].empty())
		return false;
	Graphic::Font::FontIDCount = data["font count"];

	for (UINT i = 0; i < data["font path"].size(); ++i)
	{
		if (data["font path"][i].empty())
			return false;

		path = data["font path"][i];
		wpath.assign(path.begin(), path.end());

		Graphic::Font::FontFileList.insert(std::make_pair(static_cast<Graphic::Font::ID>(i+1), wpath));
	}

	return true;
}
