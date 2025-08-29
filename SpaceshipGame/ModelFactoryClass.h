#pragma once

#include <functional>

namespace Graphic
{
	namespace Model { class IModelClass; }
}

namespace Graphic
{
	namespace Loader
	{

#ifdef _DEBUG
		std::unique_ptr<Model::IModelClass> LoadCollision(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath);
#endif
		std::unique_ptr<Model::IModelClass> LoadDefaultSpaceship(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, const std::string& ModelFilename, const std::wstring& AdditionalPath);

		class ModelFactoryClass
		{
		private:
			const UINT m_ModelIDCount = 3;
			std::map<Model::ID, const std::string> m_ModelFileList =
			{
				{ Model::ID::DEFAULT_SPACESHIP, "./resource/E-45-Aircraft/E 45 Aircraft_obj.obj" },
				{ Model::ID::COLLISION, "./resource/uploads_files_3862208_Cube.obj" },
			};

			std::map<Model::ID, const std::wstring> m_ModelAdditionPathList =
			{
				{ Model::ID::DEFAULT_SPACESHIP, _T("./resource/E-45-Aircraft/") },
				{ Model::ID::COLLISION, _T("")},
			};

		public:
			ModelFactoryClass();
			~ModelFactoryClass();

			UINT GetModelIDCount() const { return m_ModelIDCount; }
			std::unique_ptr<Model::IModelClass> Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Model::ID ModelID) const;

		private:
			static bool IsInitialize;
			std::map<Model::ID, std::function<std::unique_ptr<Model::IModelClass>(ID3D11Device*, ID3D11DeviceContext*, const std::string&, const std::wstring&)>> m_Creator;
		};
	}
}