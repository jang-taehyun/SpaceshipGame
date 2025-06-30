#pragma once

/**
* ActorClass °³¿ä
*/

#include "MoveableObjectClass.h"

namespace Object
{
	class ActorClass : public MoveableObjectClass
	{
	public:
		ActorClass(const AffineInfo& affine, std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate, std::unique_ptr<IObjectClass> collision, Graphic::Model::ID ModelID);
		ActorClass(const ActorClass& other);
		ActorClass(ActorClass&& other) noexcept;
		virtual ~ActorClass() = default;

		virtual void Move(MoveState state, float frame_time, bool IsKeyDown) override;
		virtual void Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown) override;

		inline Graphic::Model::ID GetModelID() const { return m_ModelID; }
		inline IObjectClass* GetCollision() const { return m_Collision.get(); }

		ActorClass& operator=(const ActorClass& other);
		ActorClass& operator=(ActorClass&& other) noexcept;

		virtual inline std::unique_ptr<IObjectClass> Clone() const override;

	private:
		std::unique_ptr<IObjectClass> m_Collision = nullptr;
		Graphic::Model::ID m_ModelID = Graphic::Model::ID::DEFAULT_SPACESHIP;
	};
}