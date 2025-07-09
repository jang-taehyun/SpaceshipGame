#pragma once

#include <Audio.h>

namespace Sound { class SoundClass; }

namespace Sound
{
	class SoundManagerClass
	{
	public:
		SoundManagerClass();
		virtual ~SoundManagerClass();

		void Load(UINT SoundMask);
		void Play(ID SoundID) const;
		void Frame() const;

		inline UINT GetSoundMask() const { return m_CurrentSoundMask; }

	private:
		void Initailize();
		void Shutdown();

	private:
		static bool IsInitailize;

		std::unique_ptr<DirectX::AudioEngine> m_AudioEngine = nullptr;
		std::map<ID, std::unique_ptr<SoundClass>> m_SoundList;
		UINT m_CurrentSoundMask = 0;

#ifdef DEBUG
	public:
		inline UINT GetCurrentSoundMask() const { return m_CurrentSoundMask; }
#endif // DEBUG

	};
}
