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
		void InputData();

	private:
		void InitializeGlobalVariable(nlohmann::json& data);		// json 파일이 있는 경우
		void InitializeGlobalVariable();							// json 파일이 없는 경우

	private:
		void InitializeResolution(nlohmann::json& data);
		void InitializeGraphicSetting(nlohmann::json& data);
		void InitializeInputSensitivity(nlohmann::json& data);

		// json 파일 쓰기 //
	public:
		void OutputData();

	private:
		void EditGlobalVariable(nlohmann::json& data);

	private:
		void EditResolution(nlohmann::json& data);
		void EditGraphicSetting(nlohmann::json& data);
		void EditMouseSensitivity(nlohmann::json& data);
	};
}
