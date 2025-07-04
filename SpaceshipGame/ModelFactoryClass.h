#pragma once

#include <functional>

namespace Graphic
{
	namespace Model
	{
		class IModelClass;
	}
}

namespace Graphic
{
	namespace Loader
	{
		std::unique_ptr<Model::IModelClass> LoadDefaultSpaceship(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);

		class ModelFactoryClass
		{
		public:
			ModelFactoryClass();
			~ModelFactoryClass();

			std::unique_ptr<Model::IModelClass> Load(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, Model::ID ModelID) const;

		private:
			static bool IsInitialize;
			std::map<Model::ID, std::function<std::unique_ptr<Model::IModelClass>(HWND, ID3D11Device*, ID3D11DeviceContext*)>> m_Creator;
		};
	}
}