#pragma once

#include <nlohmann/json.hpp>

namespace System
{
	class GlobalVariableInitializerClass
	{
	public:
		GlobalVariableInitializerClass() = default;
		virtual ~GlobalVariableInitializerClass() = default;

		// json 파일 읽기 //
	public:
		bool InputData();

	private:
		bool InitializeGlobalVariable(nlohmann::json& data);

	private:
		bool InitializeResolution(nlohmann::json& data);
		bool InitializeGraphicSetting(nlohmann::json& data);
		bool InitializeInputSensitivity(nlohmann::json& data);

		// json 파일 쓰기 //
	public:
		bool OutputData();

	private:
		bool EditGlobalVariable(nlohmann::json& data);

	private:
		bool EditResolution(nlohmann::json& data);
		bool EditGraphicSetting(nlohmann::json& data);
		bool EditMouseSensitivity(nlohmann::json& data);
	};
}
