#pragma once

/**
* ActorClass °³¿ä
*/

#include "GameObjectClass.h"

namespace Object
{
	class ActorClass : public GameObjectClass
	{
	public:
		ActorClass(std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate, std::unique_ptr<IObjectClass> collision, Graphic::Model::ID ModelID);
		ActorClass(const ActorClass& other);
		ActorClass(ActorClass&& other) noexcept;
		virtual ~ActorClass() = default;

		virtual void Move(MoveState state, float frame_time, bool IsKeyDown);
		virtual void Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown);

		ActorClass& operator=(const ActorClass& other);
		ActorClass& operator=(ActorClass&& other) noexcept;

		virtual inline std::unique_ptr<IObjectClass> Clone() const override { return std::make_unique<ActorClass>(*this); }

	private:
		std::unique_ptr<IMoveClass> m_Move = nullptr;
		std::unique_ptr<IRotateClass> m_Rotate = nullptr;
	};
}