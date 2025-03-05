#pragma once

class TimerClass
{
public:
	TimerClass();
	TimerClass(const TimerClass& other);
	~TimerClass();

	HRESULT Initialize();
	void Frame();

	inline const float& GetTime() const { return m_FrameTime; }

private:
	INT64 m_Frequency = 0;
	float m_TicksPerMs = 0.f;
	INT64 m_StartTime = 0;
	float m_FrameTime = 0.f;
};

