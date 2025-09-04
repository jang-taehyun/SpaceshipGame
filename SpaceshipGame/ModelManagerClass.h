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
			~ModelManagerClass();

			IModelClass* GetModel(ID id);

			void Load(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext, UINT ModelMask);

			void UpdateInstanceBuffers(ID3D11DeviceContext* DeviceContext);

			UINT GetNeedShaderMask() const { return m_NeedShaderMask; }
			UINT GetModelIDCount() const;
			UINT GetCurrentLoadedModelIDCount() const { return static_cast<UINT>(m_CurrentLoadedModelIDList.size()); }
			ID GetModelID(int idx) const;

		private:
			static bool IsInitialize;

			std::map<ID, std::unique_ptr<IModelClass>> m_ModelList;
			std::vector<ID> m_CurrentLoadedModelIDList;
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