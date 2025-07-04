#pragma once

/**
* ActorManagerClass °³¿ä
*/

namespace Object
{ 
	class IObjectClass;
	class ObjectFactoryClass;
}

namespace Object
{
	class ObjectManagerClass
	{
	public:
		ObjectManagerClass();
		virtual ~ObjectManagerClass();

		inline UINT GetPlayerIdx() const { return m_PlayerIdx; }

		inline void SetPlayerIdx(UINT idx) { m_PlayerIdx = idx; }
		inline UINT GetObjectCount() const { return static_cast<UINT>(m_ObjectList.size()); }

		IObjectClass* operator[](int idx) const;
		inline UINT GetModelMask() const { return m_ModelMask; }

		IObjectClass* Load(ID ObjectID, Graphic::Model::ID ModelID);
		void Release();

	private:
		static bool IsInitialize;

		std::vector<std::unique_ptr<IObjectClass>> m_ObjectList;
		std::unique_ptr<ObjectFactoryClass> m_Loader = nullptr;

		UINT m_PlayerIdx = 0;
		UINT m_ModelMask = 0;

	public:
		ObjectManagerClass(const ObjectManagerClass& other) = delete;
		ObjectManagerClass(ObjectManagerClass&& other) = delete;
		ObjectManagerClass& operator=(const ObjectManagerClass& other) = delete;
		ObjectManagerClass& operator=(ObjectManagerClass&& other) = delete;
	};
}