#pragma once

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetFrameTime(float);
	void GetRotation(float&);

	void TurnLeft(bool);
	void TurnRight(bool);

private:
	float m_FrameTime = 0.f;
	float m_RotationY = 0.f;
	float m_LeftTurnSpeed = 0.f;
	float m_RightTurnSpeed = 0.f;
};

