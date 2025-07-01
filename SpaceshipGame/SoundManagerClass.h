#pragma once

#include <Audio.h>

namespace Sound { class SoundClass; }

namespace Sound
{
	class SoundManagerClass
	{
	public:
		SoundManagerClass();
		virtual ~SoundManagerClass() = default;

		void Frame() const;
		void Play(ID SoundID) const;

	private:
		void Initailize();
		void Shutdown();

	private:
		static bool IsInitailize;

		std::unique_ptr<DirectX::AudioEngine> m_AudioEngine = nullptr;
		std::map<ID, std::unique_ptr<SoundClass>> m_SoundList;
	};
}
