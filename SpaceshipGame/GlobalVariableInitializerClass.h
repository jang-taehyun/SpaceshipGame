#pragma once

namespace System
{
	class GlobalVariableInitializerClass
	{
	public:
		GlobalVariableInitializerClass() = default;
		virtual ~GlobalVariableInitializerClass() = default;

		bool InputData();
		void OutputData();

	private:
		void InitializeGlobalVariable(const std::wstring& field, const std::wstring& content, const std::wstring ID);

	private:

	};
}
