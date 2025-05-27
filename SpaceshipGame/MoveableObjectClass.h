#pragma once

/**
* MoveableObjectClass °³¿ä
*/

#include "GameObjectClass.h"

class IMoveClass;
class IRotateClass;

class MoveableObjectClass : public GameObjectClass
{
public:
	MoveableObjectClass() = default;
	MoveableObjectClass(IMoveClass* const& move, IRotateClass* const& rotate);
	MoveableObjectClass(IMoveClass* const&& move, IRotateClass* const&& rotate);
	virtual ~MoveableObjectClass();

	void Move(const MoveState& state, const float& frame_time, const bool& IsKeyDown);
	void Rotate(const long& MouseX, const long& MouseY, const float& frame_time, const bool& IsKeyDown);

private:
	virtual void Shutdown() override;

private:
	IMoveClass* m_Move = nullptr;
	IRotateClass* m_Rotate = nullptr;
};

