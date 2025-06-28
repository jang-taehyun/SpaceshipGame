#pragma once

namespace Graphic
{
	namespace Model
	{
		class IModelClass;
	}
}

namespace Graphic
{
	namespace Model
	{
		class ModelManagerClass
		{
		public:
			ModelManagerClass(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
			virtual ~ModelManagerClass();

			const IModelClass* GetModel(Graphic::Model::ID id) const;

		private:
			HRESULT Initailize(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
			void Shutdown();

		private:
			static bool IsInitialize;
			std::map<Graphic::Model::ID, std::unique_ptr<IModelClass>> m_ModelList;

		public:
			ModelManagerClass(const ModelManagerClass& other) = delete;
			ModelManagerClass(ModelManagerClass&& other) = delete;
			ModelManagerClass& operator=(const ModelManagerClass& other) = delete;
			ModelManagerClass& operator=(ModelManagerClass&& other) = delete;
		};
	}
}