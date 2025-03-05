#pragma once

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass& other);
	~PositionClass();

	// Getter //

	inline const float& GetRotation() const { return m_RotationY; }

	// Setter //

	inline void SetFrameTime(const float& time) { m_FrameTime = time; }
	
	void TurnLeft(const bool& IsKeyDown);
	void TurnRight(const bool& IsKeyDown);

private:
	float x = 0.f;
	float y = 0.f;

	float m_FrameTime = 0.f;
	float m_RotationY = 0.f;
	float m_LeftTurnSpeed = 0.f;
	float m_RightTurnSpeed = 0.f;
};

