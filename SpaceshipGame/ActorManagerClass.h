#pragma once

/**
* ActorManagerClass °³¿ä
*/

namespace Object { class IObjectClass; }

namespace Object
{
	class ActorManagerClass
	{
	public:
		ActorManagerClass(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const Graphic::Model::ID* ModelIDs, int ObjectCount, int PlayerIdx);
		virtual ~ActorManagerClass();

		inline IObjectClass* GetPlayerInterface() const { return m_ObjectInterfaces[m_PlayerIdx].get(); }
		inline int GetActorInterfaceCount() const { return m_ObjectCount; }

		const IObjectClass* operator[](int idx) const;

	private:
		HRESULT Initialize(const AffineInfo* ActorAffines, const AffineInfo* CollisionAffines, const Graphic::Model::ID* ModelIDs);

	private:
		static bool IsInitialize;

		std::unique_ptr<IObjectClass> m_ObjectInterfaces[OBJECT_MAX_COUNT] = { nullptr, };
		int m_ObjectCount = 0;
		int m_PlayerIdx = 0;

	public:
		ActorManagerClass(const ActorManagerClass& other) = delete;
		ActorManagerClass(ActorManagerClass&& other) = delete;
		ActorManagerClass& operator=(const ActorManagerClass& other) = delete;
		ActorManagerClass& operator=(ActorManagerClass&& other) = delete;
	};
}