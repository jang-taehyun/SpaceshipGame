#pragma once

#include <Audio.h>

namespace Sound
{
	class SoundClass
	{
	public:
		SoundClass(DirectX::AudioEngine* engine, ID SoundID);
		SoundClass(SoundClass&& other) noexcept;
		virtual ~SoundClass();

		SoundClass& operator=(SoundClass&& other) noexcept;

		void Play() const;
		void Stop() const;

		inline void SetLoop(bool IsLoop) { m_IsLoop = IsLoop; }

	private:
		void LoadWaveFile(DirectX::AudioEngine* engine);

	private:
		ID m_ID = ID::NONE;
		bool m_IsLoop = false;
		std::unique_ptr<DirectX::SoundEffect> m_Effect = nullptr;
		std::unique_ptr<DirectX::SoundEffectInstance> m_EffectInstance = nullptr;

		// SoundEffectInstance는 복사 생성자, 복사 대입 연산자 호출이 불가능하므로 복사는 불가하게 설정 //
	public:
		SoundClass(const SoundClass& other) = delete;
		SoundClass& operator=(const SoundClass& other) = delete;
	};
}