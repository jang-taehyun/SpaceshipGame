#pragma once

/**
* MoveableObjectClass °³¿ä
*/

#include "GameObjectClass.h"

class IMoveClass;
class IRotateClass;

namespace Object
{
	class MoveableObjectClass : public GameObjectClass
	{
	public:
		MoveableObjectClass(const AffineInfo& affine, std::unique_ptr<IMoveClass> move, std::unique_ptr<IRotateClass> rotate);
		MoveableObjectClass(const MoveableObjectClass& other);
		MoveableObjectClass(MoveableObjectClass&& other) noexcept;

		virtual ~MoveableObjectClass() = default;

		virtual void Move(MoveState state, float frame_time, bool IsKeyDown);
		virtual void Rotate(long MouseX, long MouseY, float frame_time, bool IsKeyDown);

		MoveableObjectClass& operator=(const MoveableObjectClass& other);
		MoveableObjectClass& operator=(MoveableObjectClass&& other) noexcept;

		virtual inline std::unique_ptr<IObjectClass> Clone() const override;

	public:
		inline IMoveClass* GetMoveObject() const { return m_Move.get(); }
		inline IRotateClass* const& GetRotateObject() const { return m_Rotate.get(); }

	private:
		std::unique_ptr<IMoveClass> m_Move = nullptr;
		std::unique_ptr<IRotateClass> m_Rotate = nullptr;
	};
}