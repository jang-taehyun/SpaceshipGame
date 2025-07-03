#pragma once

/**
* ActorManagerClass °³¿ä
*/

namespace Object { class IObjectClass; }

namespace Object
{
	class ObjectManagerClass
	{
	public:
		ObjectManagerClass();
		virtual ~ObjectManagerClass();

		inline UINT GetPlayerIdx() const { return m_PlayerIdx; }

		inline void SetPlayerIdx(UINT idx) { m_PlayerIdx = idx; }
		inline UINT GetObjectCount() const { return static_cast<UINT>(m_Objects.size()); }

		IObjectClass* operator[](int idx) const;
		inline UINT GetModelIDs() const { return m_ModelIDs; }
		inline UINT GetShaderIDs() const { return m_ShaderIDs; }

		UINT Load(const AffineInfo& ActorAffine, const AffineInfo& CollisionAffine, Graphic::Model::ID ModelID, Graphic::Shader::ID ShaderID);
		void Release();

	private:
		static bool IsInitialize;

		std::vector<std::unique_ptr<IObjectClass>> m_Objects;
		UINT m_PlayerIdx = 0;
		UINT m_ModelIDs = 0;
		UINT m_ShaderIDs = 0;

	public:
		ObjectManagerClass(const ObjectManagerClass& other) = delete;
		ObjectManagerClass(ObjectManagerClass&& other) = delete;
		ObjectManagerClass& operator=(const ObjectManagerClass& other) = delete;
		ObjectManagerClass& operator=(ObjectManagerClass&& other) = delete;
	};
}