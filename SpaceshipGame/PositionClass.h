#pragma once

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass& other);
	~PositionClass();

	// Getter //

	inline void GetRotation(float& const y) { y = m_RotationY; }

	// Setter //

	inline void SetFrameTime(const float& const time) { m_FrameTime = time; }
	
	void TurnLeft(const bool& const IsKeyDown);
	void TurnRight(const bool& const IsKeyDown);

private:
	float m_FrameTime = 0.f;
	float m_RotationY = 0.f;
	float m_LeftTurnSpeed = 0.f;
	float m_RightTurnSpeed = 0.f;
};

