#pragma once

class TimerClass
{
public:
	TimerClass();
	~TimerClass();

	void Frame();

	inline const float& GetTime() const { return m_FrameTime; }

private:
	HRESULT Initialize();

private:
	static bool IsInitialize;

	INT64 m_Frequency = 0;
	float m_TicksPerMs = 0.f;
	INT64 m_StartTime = 0;
	float m_FrameTime = 0.f;

public:
	TimerClass(const TimerClass& other) = delete;
};

bool TimerClass::IsInitialize = false;

