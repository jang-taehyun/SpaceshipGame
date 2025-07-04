#pragma once

namespace Graphic
{
	namespace Model { class IModelClass; }
	namespace Loader { class ModelFactoryClass; }
}

namespace Graphic
{
	namespace Model
	{
		class ModelManagerClass
		{
		public:
			ModelManagerClass();
			virtual ~ModelManagerClass();

			IModelClass* GetModel(Graphic::Model::ID id) const;

			void Load(HWND hwnd, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT ModelMask);

			inline UINT GetNeedShaderMask() const { return m_NeedShaderMask; }

		private:
			static bool IsInitialize;

			std::map<Graphic::Model::ID, std::unique_ptr<IModelClass>> m_ModelList;
			std::unique_ptr<Loader::ModelFactoryClass> m_Factory = nullptr;
			UINT m_CurrentModelMask = 0;
			UINT m_NeedShaderMask = 0;

		public:
			ModelManagerClass(const ModelManagerClass& other) = delete;
			ModelManagerClass(ModelManagerClass&& other) = delete;
			ModelManagerClass& operator=(const ModelManagerClass& other) = delete;
			ModelManagerClass& operator=(ModelManagerClass&& other) = delete;
		};
	}
}