#pragma once

namespace System
{
	class TimerClass
	{
	public:
		TimerClass();
		virtual ~TimerClass();

		void Frame();

		float GetTime() const { return (m_Frequency ? m_FrameTime : -1.f); }

	private:
		void Initialize();

	private:
		static bool IsInitialize;

		INT64 m_Frequency = 0;
		float m_TicksPerMs = 0.f;
		INT64 m_StartTime = 0;
		float m_FrameTime = 0.f;

	public:
		TimerClass(const TimerClass& other) = delete;
		TimerClass(TimerClass&& other) = delete;
		TimerClass& operator=(const TimerClass& other) = delete;
		TimerClass& operator=(TimerClass&& other) = delete;
	};
}