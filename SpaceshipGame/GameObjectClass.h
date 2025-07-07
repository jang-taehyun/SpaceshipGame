#pragma once

#include "ObjectClass.h"

namespace Object
{
	class GameObjectClass : public ObjectClass
	{
	public:
		GameObjectClass(Graphic::Model::ID ModelID, std::unique_ptr<IObjectClass> collision);
		GameObjectClass(const GameObjectClass& other);
		GameObjectClass(GameObjectClass&& other) noexcept;
		virtual ~GameObjectClass() = default;

		GameObjectClass& operator=(const GameObjectClass& other);
		GameObjectClass& operator=(GameObjectClass&& other) noexcept;

		inline IObjectClass* GetCollision() const { return m_Collision.get(); }
		inline Graphic::Model::ID GetModelID() const { return m_ModelID; }

	private:
		std::unique_ptr<IObjectClass> m_Collision = nullptr;
		Graphic::Model::ID m_ModelID = Graphic::Model::ID::NONE;
	};
}
