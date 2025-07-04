#pragma once

#include <Audio.h>

namespace Sound
{
	class SoundClass
	{
	public:
		SoundClass(DirectX::AudioEngine* engine, ID SoundID);
		virtual ~SoundClass();

		void Play() const;

		inline void SetLoop(bool IsLoop) { m_IsLoop = IsLoop; }

	private:
		void LoadWaveFile(DirectX::AudioEngine* engine);

	private:
		ID m_ID = ID::NONE;
		bool m_IsLoop = false;
		std::unique_ptr<DirectX::SoundEffect> m_Effect;
		std::unique_ptr<DirectX::SoundEffectInstance> m_EffectInstance;

	public:
		SoundClass(const SoundClass& other) = delete;
		SoundClass(SoundClass&& other) = delete;
		SoundClass& operator=(const SoundClass& other) = delete;
		SoundClass& operator=(SoundClass&& other) = delete;
	};
}